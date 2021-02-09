#ifndef _MEMORYMANAGEMENT_H_
#define _MEMORYMANAGEMENT_H_

#include <Types.hpp>

#define MEMORY_START 0xF0
#define MEMORY_NEXT 0xE0
#define MEMORY_USABLE_STARTADDRESS 0x100000
#define MEMORY_STARTADDRESS 0xC00000
#define E820_STARTADDRESS 0x8000

#define MEMORY_BLOCK_NOT_ALLOCATED 0xFF
#define MEMORY_BLOCK_256B 256
#define MEMORY_BLOCK_512B 512
#define MEMORY_BLOCK_1KB 1024
#define MEMORY_BLOCK_2KB 2048
#define MEMORY_BLOCK_4KB 4096

#pragma pack(push , 1)

namespace System {
    namespace Memory {
        struct E820 {
            unsigned long BaseAddress;
            unsigned long Length;
            unsigned int Type;
            unsigned int ACPI;
        };
        struct BLOCK {
            unsigned char Allocated;
            unsigned long Address;
            unsigned char Padding;
        };
        class Management {
            public:
                unsigned long Initialize(unsigned long StartAddress , unsigned long EndAddress);
                bool Allocated(void *Address);
                unsigned long GetTotalSize(void);
                unsigned long GetUsingBlock(void);

                unsigned long malloc(unsigned long Size);
                void free(void *Address);

                unsigned long TotalSize;
                bool Usable = false;
            protected:
                unsigned long StartAddress;
                unsigned long EndAddress;
                unsigned long UsingBlockCount;
                inline unsigned long PhysicalAddressToBlockIndex(unsigned long Address) {
                    return ((Address-StartAddress)/BlockSize);
                }
            private:
                BLOCK *Blocks;
                unsigned long BlockStartAddress;
                unsigned long BlockEndAddress;
                unsigned long BlockCount;
                unsigned long BlockSize;
                unsigned long BlockAddress;
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