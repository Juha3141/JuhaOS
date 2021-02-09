#include <Paging.h>

/* 
 This code was inspired by mint64os
 (I used mint64os's code, the github link is here: 
 https://github.com/kkamagui/mint64os)
 */

void InitPML4(unsigned int PML4StartAddress) {
    unsigned int i;
    unsigned int Flag;
    PML4TABLEENTRY *PML4TEntry;
    PDPTENTRY *PDPTEntry;
    PAGEDIRECTORYENTRY *PageDirectoryEntry;
    unsigned int MappingAddress;

    PML4TEntry = (PML4TABLEENTRY*)PML4StartAddress;
    SetPageEntry(&(PML4TEntry[0]) , 0x00 , (PML4StartAddress+0x1000) , PAGE_DEFAULT , 0x00);

    for(i = 1; i < PAGE_MAXENTRYCOUNT; i++) {
        SetPageEntry(&(PML4TEntry[i]) , 0x00 , 0x00 , 0x00 , 0x00);
    }
    PDPTEntry = (PDPTENTRY*)(PML4StartAddress+0x1000);
    for(i = 0; i < 64; i++) {
        SetPageEntry(&(PDPTEntry[i]) , 0x00 , (PML4StartAddress+0x2000)+(i*PAGE_TABLESIZE) , PAGE_DEFAULT , 0x00);
    }
    for(i = 64; i < PAGE_MAXENTRYCOUNT; i++) {
        SetPageEntry(&(PDPTEntry[i]) , 0x00 , 0x00 , 0x00 , 0x00);
    }
    PageDirectoryEntry = (PAGEDIRECTORYENTRY*)(PML4StartAddress+0x2000);
    MappingAddress = 0;
    for(i = 0; i < PAGE_MAXENTRYCOUNT*64; i++) {
        SetPageEntry(&(PageDirectoryEntry[i]) , (i*(PAGE_DEFAULTSIZE >> 20)) >> 12 , MappingAddress , PAGE_DEFAULT|PAGE_PS , 0x00);
        MappingAddress += PAGE_DEFAULTSIZE;
    }
}

void SetPageEntry(PAGETABLEENTRY *Entry , unsigned int BaseAddressHigh , unsigned int BaseAddressLow , unsigned int FlagsLow , unsigned int FlagsHigh) {
    Entry->AttribPlusBaseAddressLow = BaseAddressLow|FlagsLow;
    Entry->BaseAddressHighPlusEXB = (BaseAddressHigh & 0xFF)|FlagsHigh;
}