#include <Types.h>
#include <Paging.h>
#include <Assembly.h>

#define KERNEL64_STARTADDRESS 0x200000
#define SOUL_STARTADDRESS 0x400000
#define FAT_ROOTENTRYCOUNT 224

#define VBEMODE 0x117

#pragma pack(push , 1)

typedef struct {
    char Name[8];
    char EXT[3];
    unsigned char FileProperty;
    unsigned char Reserved[10];
    unsigned short Time;
    unsigned short Date;
    unsigned short ClusterStartAddress;
    unsigned int FileSize;
}FATROOTENTRY;

typedef struct {
    unsigned short DI;
    unsigned short SI;
    unsigned short BP;
    unsigned short SP;
    unsigned short BX;
    unsigned short DX;
    unsigned short CX;
    unsigned short AX;

    unsigned short GS;
    unsigned short FS;
    unsigned short ES;
    unsigned short DS;
    unsigned short EFlags;
}REGISTERS;

#pragma pack(pop)

void PrintString(int X , int Y , const char *Buffer);
void ReadSector(int SectorNumber , unsigned char SectorCountToRead , unsigned char DriveNumber);
FATROOTENTRY *FindFile(unsigned char *FileName);
void ReadOneSector(int SectorNumber , unsigned char *Buffer);

void Main32(void) {
    int i;
    int SectorNumber;
    int SectorCountToRead;
    unsigned char *Kernel64Address = (unsigned char*)KERNEL64_STARTADDRESS;
    unsigned char *SoulAddress = (unsigned char*)SOUL_STARTADDRESS;
    unsigned char *TextScreenBuffer = (unsigned char*)0xB8000;
    REGISTERS Registers;
    FATROOTENTRY *Kernel64;
    FATROOTENTRY *SOUL;
    for(i = 0; i < 80*25; i++) {
        *TextScreenBuffer++ = 0x00;
        *TextScreenBuffer++ = 0x07;
    }
    Kernel64 = FindFile("KERNEL64BIN");
    SOUL = FindFile("SOUL       ");
    if(Kernel64 == NULL) {
        PrintString(0 , 0 , "I can't find the Kernel.");
        while(1) {
            ;
        }
    }
    if(SOUL == NULL) {
        PrintString(0 , 0 , "I can't find the Main Kernel.");
        while(1) {
            ;
        }
    }
    SectorCountToRead = (Kernel64->FileSize/512)+((Kernel64->FileSize%512 != 0x00) ? 1 : 0);
    SectorNumber = Kernel64->ClusterStartAddress+31;
    for(i = 0; i < SectorCountToRead; i++) {
        ReadOneSector(SectorNumber , Kernel64Address);
        SectorNumber += 1;
        Kernel64Address += 0x200;
    }

    SectorCountToRead = (SOUL->FileSize/512)+((SOUL->FileSize%512 != 0x00) ? 1 : 0);
    SectorNumber = SOUL->ClusterStartAddress+31;
    for(i = 0; i < SectorCountToRead; i++) {
        ReadOneSector(SectorNumber , SoulAddress);
        SectorNumber += 1;
        SoulAddress += 0x200;
    }

    if(Check64BitSupported() == false) { 
        PrintString(0 , 0 , "64bit isn't supporting in this PC :(");
        while(1) {
            ;
        }
    }
    InitPML4(0x12000);
    SwitchTo64BitAndJump();
    while(1) { 
        ;
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

FATROOTENTRY *FindFile(unsigned char *FileName) {
    int i;
    int j;
    bool Found;
    FATROOTENTRY *RootEntry = (FATROOTENTRY*)0x600;
    for(i = 0; i < FAT_ROOTENTRYCOUNT; i++) {
        Found = true;
        for(j = 0; j < 11; j++) {
            if(FileName[j] != RootEntry[i].Name[j]) {
                Found = false;
                break;
            }
        }
        if(Found == true) {
            break;
        }
    }
    if(Found == true) {
        return &(RootEntry[i]);
    }
    else {
        return (FATROOTENTRY*)0x00;
    }
}

void ReadOneSector(int SectorNumber , unsigned char *Buffer) {
    int i;
    REGISTERS Registers;
    unsigned char *RealBuffer = (unsigned char*)0x3000;
    unsigned char Track = (unsigned char)(SectorNumber/(18*2));
    unsigned char Head = (unsigned char)(SectorNumber/18)%2;
    unsigned char Sector = (unsigned char)(SectorNumber%18)+1;

    Registers.ES = 0x300;
    Registers.BX = 0x00;

    Registers.AX = 0x0201;
    Registers.CX = (Track << 8)+Sector;
    Registers.DX = 0x00+(Head << 8);
    int32(0x13 , &(Registers));

    for(i = 0; i < 512; i++) {
        Buffer[i] = RealBuffer[i];
    }
}