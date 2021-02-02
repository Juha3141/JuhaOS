#ifndef _TYPES_H_
#define _TYPES_H_

#define OS_NAME "JuhaOS"
#define OS_VERSION "0.1.4"
#define OS_COPYRIGHT "(C) 2017-2020. all rights reserved by Juha3141."

#define true 1
#define false 0
#define NULL 0x00
#define MIN(X , Y) ((X) < (Y)) ? (X) : (Y)
#define MAX(X , Y) ((X) > (Y)) ? (X) : (Y)

#define QUEUE_MAXSIZE 4096

class Queue {
	public:
		void Initialize(int MaxSize) {
		    if(MaxSize > QUEUE_MAXSIZE) {
		        MaxSize = QUEUE_MAXSIZE;
		    }
		    this->MaxSize = MaxSize;
		    this->Rear = 0;
		    this->Front = 0;
		}
		bool CheckEmpty(void) {
			if(Rear == Front) {
				return true;
			}
			return false;
		}
		bool CheckFull(void) {
			if((Rear+1)%MaxSize == Front) {
				return true;
			}
			return false;
		}
		bool Enqueue(unsigned char Data) {
			if(this->CheckFull() == true) {
				return false;
			}
			Rear = (Rear+1)%MaxSize;
			Buffer[Rear] = Data;
			return true;
		}
		unsigned char Dequeue(void) {
			if(this->CheckEmpty() == true) {
				return 0;
			}
			Front = (Front+1)%MaxSize;
			return Buffer[Front];
		}
	private:
		int MaxSize;
		int Front;
		int Rear;
		int Buffer[QUEUE_MAXSIZE];
};

#pragma pack(push , 1)

struct VBEMODEINFOBLOCK {
	unsigned short Attributes;
	unsigned char WinA;
	unsigned char WinB;
	unsigned short Granularity;
	unsigned short WinSize;
	unsigned short SegmentA;
	unsigned short SegmentB;
	unsigned int ForRealMode1;
	unsigned short charsPerScanLine;
	unsigned short Width;
	unsigned short Height;	
	unsigned char WidthCharSize;
	unsigned char HeightCharSize;
	unsigned char NumberOfPlane;
	unsigned char BitsPerPixel;
	unsigned char NumberOfBanks;
	unsigned char MemoryModel;
	unsigned char BankSize;
	unsigned char NumberOfImagedPages;
	unsigned char Reserved1;

	unsigned char RedMaskSize;
	unsigned char RedFieldPosition;
	unsigned char GreenMaskSize;
	unsigned char GreenFieldPosition;
	unsigned char BlueMaskSize;
	unsigned char BlueFieldPosition;

	unsigned char ReservedMaskSize;
	unsigned char ReservedFieldPosition;
	unsigned char DirectColorModeInfo;

	unsigned int Address;
	unsigned int Reserved2;
	unsigned int Reserved3;

	unsigned short LinearBytesPerScanLine;
	unsigned char BankNumberOfImagePages;
    unsigned char LinearNumberOfImagePages;
    unsigned char LinearRedMaskSize;
    unsigned char LinearRedFieldPosition;
    unsigned char LinearGreenMaskSize;
    unsigned char LinearGreenFieldPosition;
    unsigned char LinearBlueMaskSize;
    unsigned char LinearBlueFieldPosition;
    unsigned char LinearReservedMaskSize;
    unsigned char LinearReservedFieldPosition;
    unsigned int MaxPixelClock;

    unsigned char Reserved4[189];
};

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

#endif