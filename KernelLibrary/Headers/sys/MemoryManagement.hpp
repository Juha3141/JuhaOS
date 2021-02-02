#ifndef _MEMORYMANAGEMENT_H_
#define _MEMORYMANAGEMENT_H_

#include <Types.hpp>

#define MEMORY_START 0xF0
#define MEMORY_NEXT 0xE0
#define MEMORY_STARTADDRESS 0xC00000
#define MEMORY_BLOCKSIZE 256

#pragma pack(push , 1)

namespace System {
    namespace Memory {
        class BLOCK {
            public:
                unsigned char Start;
                unsigned char Allocated;
                unsigned long BlockCount;
        };
        class Management {
            public:
                unsigned long StartAddress;
                unsigned long EndAddress;
                unsigned long TotalSize;
                unsigned long BlockCount;
                unsigned long UsingBlock;
                BLOCK *Blocks;
        };

        bool Initialize(void);
        unsigned long GetTotalSize(void);
        unsigned long GetUsingBlock(void);
        bool Allocated(void *Address);
        unsigned long malloc(unsigned long Size);
        void free(void *Address);
    }
}

#pragma pack(pop)

#endif