#include <sys/MemoryManagement.hpp>
#include <sys/Hardware.hpp>
#include <sys/DescriptorTables.hpp>
#include <TextScreen.hpp>
#include <string.hpp>

using namespace System;

static Memory::Management MemoryManager;

bool Memory::Initialize(void) {
    unsigned long i;
    unsigned long *MemoryLocation;
    unsigned long TempBuffer;
    MemoryLocation = (unsigned long*)MEMORY_STARTADDRESS;
    __asm__ ("cli");
    while(1) {
        TempBuffer = (*MemoryLocation);
        (*MemoryLocation) = 0x12345678AABBCCDD;
        if(((unsigned long)(*MemoryLocation)) != 0x12345678AABBCCDD) {
            break;
        }
        (*MemoryLocation) = TempBuffer;
        MemoryLocation += MEMORY_BLOCKSIZE;
    }
    MemoryManager.Blocks = (Memory::BLOCK*)MEMORY_STARTADDRESS;
    MemoryManager.TotalSize = (unsigned long)MemoryLocation-MEMORY_STARTADDRESS;
    MemoryManager.StartAddress = MEMORY_STARTADDRESS+(MemoryManager.TotalSize/MEMORY_BLOCKSIZE);
    MemoryManager.TotalSize -= (MemoryManager.TotalSize/MEMORY_BLOCKSIZE);
    MemoryManager.BlockCount = (unsigned long)(MemoryManager.TotalSize/MEMORY_BLOCKSIZE);
    MemoryManager.UsingBlock = 0x00;
    for(i = 0; i < (unsigned long)(MemoryManager.TotalSize/MEMORY_BLOCKSIZE); i++) {
        MemoryManager.Blocks[i].Start = 0x00;
        MemoryManager.Blocks[i].Allocated = false;
        MemoryManager.Blocks[i].BlockCount = 0x00;
    }
    __asm__ ("sti");
    return true;
}

bool Memory::Allocated(void *Address) {
    unsigned long i;
    unsigned long BlockAddress;
    __asm__ ("cli");
    BlockAddress = (unsigned long)((((unsigned long)Address)-MemoryManager.StartAddress)/(MEMORY_BLOCKSIZE));
    if((MemoryManager.Blocks[BlockAddress].Start == false) && (MemoryManager.Blocks[BlockAddress].Allocated == true)) {
        __asm__ ("sti");
        return false;
    }
    __asm__ ("sti");
    return true;
}

unsigned long Memory::GetTotalSize(void) {
    return MemoryManager.TotalSize;
}

unsigned long Memory::GetUsingBlock(void) {
    return MemoryManager.UsingBlock;
}

unsigned long Memory::malloc(unsigned long Size) {
    unsigned long i;
    int TempColor;
    unsigned long UsingBlock;
    unsigned long Address = MemoryManager.StartAddress;
    unsigned long StartBlock;
    bool Founded = false;
    __asm__ ("cli");
    if(Size < MEMORY_BLOCKSIZE) {
        UsingBlock = 1;
    }
    else {
        if(Size%MEMORY_BLOCKSIZE != 0x00) {
            UsingBlock = (Size/MEMORY_BLOCKSIZE)+1;
        }
        else {
            UsingBlock = Size/MEMORY_BLOCKSIZE;
        }
    }
    MemoryManager.UsingBlock += UsingBlock;
    for(i = 0; i < MemoryManager.BlockCount; i++) {
        if(MemoryManager.Blocks[i].Allocated == false) {
            if(MemoryManager.Blocks[i].Start == true) {
                if(MemoryManager.Blocks[i].BlockCount < UsingBlock) {
                    i++;
                    continue;
                }
            }
            MemoryManager.Blocks[i].Allocated = true;
            MemoryManager.Blocks[i].Start = true;
            MemoryManager.Blocks[i].BlockCount = UsingBlock;
            Address = (unsigned long)(MemoryManager.StartAddress+(i*MEMORY_BLOCKSIZE));
            Founded = true;
            StartBlock = i;
            break;
        }
    }
    if(Founded == false) {
        TempColor = TextScreen::GetColor();
        TextScreen::SetColor(0x04);
        TextScreen::printf("No more memory for you");
        TextScreen::SetColor(TempColor);
        __asm__ ("sti");
        return 0x00;
    }
    for(i = StartBlock+1; i < StartBlock+UsingBlock; i++) {
        MemoryManager.Blocks[i].Allocated = true;
        MemoryManager.Blocks[i].Start = false;
        MemoryManager.Blocks[i].BlockCount = 0;
    }
    __asm__ ("sti");
    return Address;
}

void Memory::free(void *Address) {
    unsigned long i;
    unsigned long BlockAddress;
    unsigned long BlockCount;
    __asm__ ("cli");
    BlockAddress = (unsigned long)((((unsigned long)Address)-MemoryManager.StartAddress)/(MEMORY_BLOCKSIZE));
    if((MemoryManager.Blocks[BlockAddress].Start == false) && (MemoryManager.Blocks[BlockAddress].Allocated == true)) {
        __asm__ ("sti");
        return;
    }
    BlockCount = MemoryManager.Blocks[BlockAddress].BlockCount;
    MemoryManager.UsingBlock -= BlockCount;
    for(i = BlockAddress; i <= BlockAddress+BlockCount; i++) {
        MemoryManager.Blocks[i].Allocated = false;
        MemoryManager.Blocks[i].Start = false;
        MemoryManager.Blocks[i].BlockCount = 0;
    }
    __asm__ ("sti");
    return;
}