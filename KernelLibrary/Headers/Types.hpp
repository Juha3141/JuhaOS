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

#endif