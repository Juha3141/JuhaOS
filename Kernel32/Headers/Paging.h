#ifndef _PAGING_H_
#define _PAGING_H_

#include <Types.h>

#define PAGE_TABLESIZE 0x1000
#define PAGE_MAXENTRYCOUNT 512
#define PAGE_DEFAULTSIZE 0x200000

#define PAGE_P 0x01
#define PAGE_RW 0x02
#define PAGE_US 0x04
#define PAGE_PWT 0x08
#define PAGE_PCD 0x10
#define PAGE_A 0x20
#define PAGE_D 0x40
#define PAGE_PS 0x80
#define PAGE_G 0x100
#define PAGE_PAT 0x1000
#define PAGE_EXB 0x80000000
#define PAGE_DEFAULT (PAGE_P|PAGE_RW)

#pragma pack(push , 1)

typedef struct {
    unsigned long AttribPlusBaseAddressLow;
    unsigned long BaseAddressHighPlusEXB;
}PML4TABLEENTRY , PML4TABLEENTRY , PDPTENTRY , PAGEDIRECTORYENTRY , PAGETABLEENTRY;

#pragma pack(pop)

void InitPML4(unsigned int PML4StartAddress);
void SetPageEntry(PAGETABLEENTRY *Entry , unsigned int BaseAddressHigh , unsigned int BaseAddressLow , unsigned int FlagsLow , unsigned int FlagsHigh);

#endif