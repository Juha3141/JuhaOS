#ifndef _MULTITASKING_H_
#define _MULTITASKING_H_

#include <Types.hpp>

#define TASK_DEFAULT 0x00
#define TASK_SYSTEM 0x01
#define TASK_NONE 0xFF

#define TASK_MAINTASKID 0x8000000
#define TASK_IDTOOFFSET(ID) (ID-TASK_MAINTASKID)

#define TASK_STACKSIZE 8192
#define TASK_MAXCOUNT 1024

namespace System {
    namespace Task {
        struct REGISTERS {
            unsigned long GS;
            unsigned long FS;
            unsigned long ES;
            unsigned long DS;
            unsigned long R15;
            unsigned long R14;
            unsigned long R13;
            unsigned long R12;
            unsigned long R11;
            unsigned long R10;
            unsigned long R9;
            unsigned long R8;
            unsigned long RSI;
            unsigned long RDI;
            unsigned long RDX;
            unsigned long RCX;
            unsigned long RBX;
            unsigned long RAX;
            unsigned long RBP;
            unsigned long RIP;
            unsigned long CS;
            unsigned long RFlags;
            unsigned long RSP;
            unsigned long SS;
        };

        class TASKINFO {
            public:
                unsigned long ID;
                unsigned long Flags;
                bool Using;

                unsigned char *Stack;
                REGISTERS Registers;

                char Name[512];
                char Description[512];
        };

        class Manager {
            public:
                int CurrentPriority;

                unsigned long CurrentTaskID;
                unsigned long NextTaskID;
                
                unsigned long CurrentTaskIndex;
                unsigned long NextTaskIndex;

                unsigned long RunningTaskCount;
                unsigned long TaskCount;
                TASKINFO Tasks[TASK_MAXCOUNT];
                unsigned long Timer;
        };

        extern "C" void SwitchRegisters(REGISTERS *Current , REGISTERS *Next);

        void Initialize(void);
        unsigned long CreateTask(unsigned long EntryPoint , unsigned long Flags , const char *Name , const char *Description);
        bool EndTask(unsigned long ID);
        void Exit(void);
        void SetupTaskRegisters(REGISTERS *Registers , unsigned char *Stack , unsigned long EntryPoint);
        TASKINFO GetTaskInfo(unsigned long ID);
        unsigned long GetCurrentTaskID(void);
        unsigned long GetRunningTaskCount(void);
        Task::TASKINFO *GetTaskList(void);
        unsigned int GetProcessorUsing(void);
        void SwitchTask(void);
        void SwitchTaskInInterrupt(void);

        unsigned long ChangeIDToOffset(unsigned long ID);

        void System(void);
        void IDLE(void);
    }
}

#endif