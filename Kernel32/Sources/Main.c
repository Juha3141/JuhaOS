#include <Types.h>
#include <Paging.h>
#include <Assembly.h>

#define KERNEL32_STARTADDRESS 0x100000
#define KERNEL64_REALADDRESS 0x100000+(KERNEL32_SECTORCOUNT*unsigned charS_PER_SECTOR)
#define KERNEL64_STARTADDRESS 0x200000

#define VBEMODE 0x117

#pragma pack(push , 1)

typedef struct {
    void *Start;
    void *End;
    char *Name;
    unsigned int Reserved;
}MODULE;

typedef struct {
    unsigned int Flags;
    unsigned int MemoryLow;
    unsigned int MemoryHigh;
    unsigned int BootDrive;
    char *CommandLine;
    unsigned int ModulesCount;
    MODULE *Modules;
}MULTIBOOTINFO;

#pragma pack(pop)

void SwitchTo64BitModeAndJump(void);
unsigned int CheckMaxMemory(void);
bool Check64BitSupported(void);

void PrintString(int X , int Y , const char *Buffer);
void LoadModule(int Number , unsigned int Address , unsigned int MultibootAddress);

void Main32(unsigned long MultibootMagic , unsigned long MultibootAddress) {
    int i = 80*25;
    unsigned char *TextScreenBuffer = (unsigned char*)0xB8000;
    unsigned int RAMSize;
    char *Buffer = (char*)0xACACAC;
    unsigned long *SaveMultibootInfo = (unsigned long*)0x12000;
    SaveMultibootInfo[0] = MultibootMagic;
    SaveMultibootInfo[1] = MultibootAddress;
    while(i --> 0) {
        *TextScreenBuffer++ = 0x00;
        *TextScreenBuffer++ = 0x07;
    }
    if((RAMSize = CheckMaxMemory()) < 512) {
        PrintString(0 , 0 , "RAM size is too low, min 512MB required :(");
        while(1) {
            ;
        }
    }
    if(Check64BitSupported() == false) {
        PrintString(0 , 0 , "64bit isn't supporting in this PC :(");
        while(1) {
            ;
        }
    }

    InitPML4(0x16000);
    LoadModule(0 , 0x200000-0x1000 , MultibootAddress);
    LoadModule(1 , 0x300000-0x1000 , MultibootAddress);
    SwitchTo64BitAndJump();
    while(1) { 
        ;
    }
}

void LoadModule(int Number , unsigned int Address , unsigned int MultibootAddress) {
    int i = 0;
    MULTIBOOTINFO *MultibootInfo = (MULTIBOOTINFO*)MultibootAddress;
    unsigned char *MemoryAddress;
    unsigned char *Buffer = (unsigned char*)Address;
    Buffer[i] = (unsigned char)MultibootInfo->Modules[Number].Start;
    for(MemoryAddress = MultibootInfo->Modules[Number].Start; (MemoryAddress < MultibootInfo->Modules[Number].End); MemoryAddress++) {
        Buffer[i++] = (unsigned char)(*MemoryAddress);
    }
}

void PrintString(int X , int Y , const char *Buffer) {
    int i;
    const int Offset = ((Y*80)+X)*2;
    char *ScreenBuffer = (char*)0xB8000+Offset;
    for(i = 0; Buffer[i] != '\0'; i++) {
        *(ScreenBuffer++) = Buffer[i];
        *(ScreenBuffer++) = 0x07;
    }
}

unsigned int CheckMaxMemory(void) {
    unsigned int Buffer;
    unsigned int *MemoryLocation = (unsigned int*)KERNEL32_STARTADDRESS;
    while(1) {
        MemoryLocation += 4*MB;
        Buffer = (unsigned int)MemoryLocation;
        *MemoryLocation = 0x31415926;
        if(*MemoryLocation != 0x31415926) {
            break;
        }
        *MemoryLocation = Buffer;
    }
    return ((unsigned int)MemoryLocation)/MB;
}