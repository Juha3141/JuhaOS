#include <sys/Task.hpp>
#include <string.hpp>
#include <TextScreen.hpp>
#include <sys/MemoryManagement.hpp>
#include <sys/DescriptorTables.hpp>
#include <sys/Hardware.hpp>

using namespace System;

static Task::Manager TaskManager;

void Task::Initialize(void) {
    int i;
    __asm__ ("cli");
    for(i = 0; i < TASK_MAXCOUNT; i++) {
        TaskManager.Tasks[i].ID = TASK_MAINTASKID+i;
        TaskManager.Tasks[i].Flags = TASK_NONE;
        TaskManager.Tasks[i].Using = false;
    }
    TaskManager.TaskCount = 1;
    TaskManager.CurrentTaskIndex = 0x00;
    TaskManager.NextTaskIndex = 0x01;

    TaskManager.Tasks[0].ID = TASK_MAINTASKID;
    TaskManager.Tasks[0].Flags = TASK_SYSTEM;
    TaskManager.Tasks[0].Using = true;
    TaskManager.Tasks[0].Stack = 0x00;
    strcpy(TaskManager.Tasks[0].Name , "MAIN");
    strcpy(TaskManager.Tasks[0].Description , "Kernel system task");
    SetupTaskRegisters(&(TaskManager.Tasks[0].Registers) , 0x00 , 0x00);
    __asm__ ("sti");
    CreateTask((unsigned long)IDLE , TASK_SYSTEM , "SYSTEM" , "A task for control/manage the system");
    CreateTask((unsigned long)System , TASK_SYSTEM , "IDLE" , "IDLE task");
}

unsigned long Task::CreateTask(unsigned long EntryPoint , unsigned long Flags , const char *Name , const char *Description) {
    int i;
    unsigned long ID;
    __asm__ ("cli");
    if(TaskManager.TaskCount >= TASK_MAXCOUNT-1) {
        __asm__ ("sti");
        return 0x00;
    }
    for(i = 0; i < TASK_MAXCOUNT; i++) {
        if(TaskManager.Tasks[i].Using == false) {
            TaskManager.Tasks[i].Using = true;
            break;
        }
    }
    ID = TaskManager.Tasks[i].ID;
    strcpy(TaskManager.Tasks[i].Name , Name);
    strcpy(TaskManager.Tasks[i].Description , Description);
    TaskManager.Tasks[i].Flags = Flags;
    TaskManager.Tasks[i].Stack = (unsigned char*)Memory::malloc(TASK_STACKSIZE);
    SetupTaskRegisters(&(TaskManager.Tasks[i].Registers) , TaskManager.Tasks[i].Stack , EntryPoint);

    __asm__ ("sti");
    delay(5);
    TaskManager.TaskCount++;
    return ID;
}

void Task::SetupTaskRegisters(Task::REGISTERS *Registers , unsigned char *Stack , unsigned long EntryPoint) {
    __asm__ ("cli");
    memset(Registers , 0 , sizeof(Registers));
    Registers->RBP = (unsigned long)Stack+TASK_STACKSIZE;
    Registers->RSP = (unsigned long)Stack+TASK_STACKSIZE;
    Registers->CS = 0x08;
    Registers->DS = 0x10;
    Registers->ES = 0x10;
    Registers->FS = 0x10;
    Registers->GS = 0x10;
    Registers->SS = 0x10;
    Registers->RIP = EntryPoint;
    Registers->RFlags |= 0x3200;
    __asm__ ("sti");
}

unsigned long Task::GetCurrentTaskID(void) {
    return TaskManager.Tasks[TaskManager.CurrentTaskIndex].ID;
}

unsigned long Task::GetRunningTaskCount(void) {
    return TaskManager.TaskCount;
}

Task::TASKINFO *Task::GetTaskList(void) {
    unsigned long i;
    Task::TASKINFO *List = (TASKINFO*)Memory::malloc(TASK_MAXCOUNT*sizeof(TASKINFO));
    List = TaskManager.Tasks;
    return List;
}


Task::TASKINFO Task::GetTaskInfo(unsigned long ID) {
    return TaskManager.Tasks[ChangeIDToOffset(ID)];
}

void Task::SwitchTask(void) {
    unsigned long CurrentTaskIndex = TaskManager.CurrentTaskIndex;
    unsigned long NextTaskIndex = TaskManager.NextTaskIndex;
    char *ContextAddress = (char*)IST_STARTADDRESS+IST_SIZE-sizeof(REGISTERS);
    __asm__ ("cli");
    if(TaskManager.TaskCount == 1) {
        __asm__ ("sti");
        return;
    }
    TaskManager.CurrentTaskIndex++;
    TaskManager.NextTaskIndex++;
    if(TaskManager.CurrentTaskIndex >= TaskManager.TaskCount) {
        TaskManager.CurrentTaskIndex = 0x00;
    }
    if(TaskManager.NextTaskIndex >= TaskManager.TaskCount) {
        TaskManager.NextTaskIndex = 0x00;
    }
    if((TaskManager.Tasks[TaskManager.NextTaskIndex].Flags == TASK_NONE)||(TaskManager.Tasks[TaskManager.NextTaskIndex].Using == false)) {
        __asm__ ("sti");
        return;
    }
    SwitchRegisters(&(TaskManager.Tasks[CurrentTaskIndex].Registers) , &(TaskManager.Tasks[NextTaskIndex].Registers));
    __asm__ ("sti");
}

void Task::SwitchTaskInInterrupt(void) {
    unsigned long CurrentTaskIndex = TaskManager.CurrentTaskIndex;
    unsigned long NextTaskIndex = TaskManager.NextTaskIndex;
    char *ContextAddress = (char*)IST_STARTADDRESS+IST_SIZE-sizeof(REGISTERS);
    if(TaskManager.TaskCount == 1) {
        return;
    }
    TaskManager.CurrentTaskIndex++;
    TaskManager.NextTaskIndex++;
    if(TaskManager.CurrentTaskIndex >= TaskManager.TaskCount) {
        TaskManager.CurrentTaskIndex = 0x00;
    }
    if(TaskManager.NextTaskIndex >= TaskManager.TaskCount) {
        TaskManager.NextTaskIndex = 0x00;
    }
    if((TaskManager.Tasks[TaskManager.NextTaskIndex].Flags == TASK_NONE)||(TaskManager.Tasks[TaskManager.NextTaskIndex].Using == false)) {
        return;
    }
    memcpy(&(TaskManager.Tasks[CurrentTaskIndex].Registers) , ContextAddress , sizeof(REGISTERS)); 
    memcpy(ContextAddress , &(TaskManager.Tasks[NextTaskIndex].Registers) , sizeof(REGISTERS));
}

bool Task::EndTask(unsigned long ID) {
    unsigned long i;
    __asm__ ("cli");
    if(!((ID >= TASK_MAINTASKID) && (ID <= TASK_MAINTASKID+TASK_MAXCOUNT))) {
        __asm__ ("sti");
        return false;
    }
    if((TaskManager.Tasks[ChangeIDToOffset(ID)].Flags == TASK_SYSTEM)||(TaskManager.Tasks[ChangeIDToOffset(ID)].Flags == TASK_NONE)) {
        __asm__ ("sti");
        return false;
    }
    TaskManager.TaskCount--;
    if(ChangeIDToOffset(ID) == TaskManager.TaskCount) {
        Memory::free(TaskManager.Tasks[ChangeIDToOffset(ID)].Stack);
        TaskManager.Tasks[ChangeIDToOffset(ID)].Flags = TASK_NONE;
        TaskManager.Tasks[ChangeIDToOffset(ID)].Using = false;
        __asm__ ("sti");
        return true;
    }
    Memory::free(TaskManager.Tasks[ChangeIDToOffset(ID)].Stack);
    for(i = ChangeIDToOffset(ID); i < TaskManager.TaskCount; i++) {
        memcpy(&(TaskManager.Tasks[i]) , &(TaskManager.Tasks[i+1]) , sizeof(TASKINFO));
    }
    Memory::free(TaskManager.Tasks[i].Stack);
    TaskManager.Tasks[i].ID = ID;
    TaskManager.Tasks[i].Flags = TASK_NONE;
    TaskManager.Tasks[i].Using = false;
    __asm__ ("sti");
    return true;
}

void Task::Exit(void) {
    Task::EndTask(Task::GetCurrentTaskID());
    while(1) {
        ;
    }
}

unsigned long Task::ChangeIDToOffset(unsigned long ID) {
    unsigned long i;
    for(i = 0; i < TASK_MAXCOUNT; i++) {
        if(ID == TaskManager.Tasks[i].ID) {
            return i;
        }
    }
    return 0x00;
}

void Task::System(void) {
    while(1) {
        ;
    }
}

void Task::IDLE(void) {
    while(1) {
        ;
    }
}