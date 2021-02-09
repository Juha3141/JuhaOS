#include <sys/MemoryManagement.hpp>
#include <sys/Hardware.hpp>
#include <sys/DescriptorTables.hpp>
#include <TextScreen.hpp>
#include <string.hpp>

/*
 * This code is made out of the tear.
 * If You are going to copy-paste this code,
 * please, just please don't delete this message.
 * you can do whatever you want with this code but, 
 * don't delete this message.
 *
 * 2021.2.9, Juha3141.
*/

using namespace System;

static Memory::Management MemoryManager[2];
static Memory::E820 *MemoryMap = (Memory::E820*)E820_STARTADDRESS;

bool Memory::Initialize(void) {
    unsigned long i = 0;
    unsigned long j = 0;
    unsigned long TotalMemory;
    
    __asm__ ("cli");
    while(i += 1) {
        if(MemoryMap[i].Type == 1) {
            if(MemoryMap[i].BaseAddress == MEMORY_USABLE_STARTADDRESS) {
                TotalMemory += MemoryManager[j++].Initialize(MEMORY_STARTADDRESS , MemoryMap[i].BaseAddress+MemoryMap[i].Length);
                MemoryManager[j].Usable = true;
                j += 1;
            }
            if(MemoryMap[i].BaseAddress > MEMORY_USABLE_STARTADDRESS) {
                TotalMemory += MemoryManager[j].Initialize(MemoryMap[i].BaseAddress , MemoryMap[i].BaseAddress+MemoryMap[i].Length);
                MemoryManager[j].Usable = true;
                j += 1;
            }
        }
        if(MemoryMap[i].Length == (unsigned long)0x00) {
            break;
        }
    }

    TotalMemory -= MEMORY_STARTADDRESS;
    TextScreen::printf("Total memory : %dMB\n" , TotalMemory/1024/1024);

    while(1) {
        ;
    }
    __asm__ ("sti");
    return true;
}

static unsigned long GetBlockMode(unsigned long StartAddress , unsigned long EndAddress) {
    const static unsigned long MB = 1024*1024;
    unsigned long MemorySize = EndAddress-StartAddress;
    if((MemorySize > 0) && (MemorySize <= 1024*MB)) {
        return MEMORY_BLOCK_256B;
    }
    if((MemorySize > 1024*MB) && (MemorySize <= 2048*MB)) {
        return MEMORY_BLOCK_512B;
    }
    if((MemorySize > 2048*MB) && (MemorySize <= 4096*MB)) {
        return MEMORY_BLOCK_1KB;
    }
    if((MemorySize > 4096*MB) && (MemorySize <= 8192*MB)) {
        return MEMORY_BLOCK_2KB;
    }
    if(MemorySize > 8192*MB) {
        return MEMORY_BLOCK_4KB;
    }
    return 0x00;
}

unsigned long Memory::Management::Initialize(unsigned long StartAddress , unsigned long EndAddress) {
    unsigned long i;
    unsigned long j;
    unsigned long BlockSize = GetBlockMode(StartAddress , EndAddress);
    this->BlockCount = (EndAddress-StartAddress)/BlockSize;
    this->BlockStartAddress = StartAddress;
    this->BlockEndAddress = StartAddress+(BlockCount*sizeof(BLOCK))-1;
    this->StartAddress = StartAddress+(BlockCount*sizeof(BLOCK));
    this->EndAddress = EndAddress;
    this->BlockSize = BlockSize;;
    this->Blocks = (BLOCK*)this->BlockStartAddress;
    TextScreen::printf("Usable              : 0x%X - 0x%X\n" , this->StartAddress , this->EndAddress);
    TextScreen::printf("Block Start Address : 0x%X - 0x%X\n" , this->BlockStartAddress , this->BlockEndAddress);
    TextScreen::printf("Block Count         : %d\n" , this->BlockCount);
    TextScreen::printf("Block Size          : %d\n" , this->BlockSize);

    for(i = 0; i < BlockCount; i++) {
        Blocks[i].Allocated = MEMORY_BLOCK_NOT_ALLOCATED;
        Blocks[i].Address = (this->StartAddress+(i*BlockSize));
    }
    this->UsingBlockCount = 0;
    return this->EndAddress-this->StartAddress;
}

bool Memory::Management::Allocated(void *Address) {
    return (this->Blocks[PhysicalAddressToBlockIndex((unsigned long)Address)].Allocated == MEMORY_BLOCK_NOT_ALLOCATED) ? false : true;
}

unsigned long Memory::Management::GetTotalSize(void) {
    return this->UsingBlockCount*this->BlockSize;
}

unsigned long Memory::Management::GetUsingBlock(void) {
    return this->UsingBlockCount;
}

unsigned long Memory::Management::malloc(unsigned long Size) {
    ;
}

void Memory::Management::free(void *Address) {
    ;
}



unsigned long Memory::GetTotalSize(void) {

}

unsigned long Memory::GetUsingBlock(void) {
    
}

unsigned long Memory::malloc(unsigned long Size) {

}

void Memory::free(void *Address) {
    
}