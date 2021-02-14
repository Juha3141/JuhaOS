#ifndef _DESCRIPTORTABLES_H_
#define _DESCRIPTORTABLES_H_

#include <Types.hpp>

#define TSS_STARTADDRESS 0x800000
#define IST_STARTADDRESS 0x900000
#define IST_SIZE 0x100000

namespace System {
    namespace DescriptorTables {
        class GDTEntry {
            public:
                unsigned short LimitLow;
                unsigned short BaseAddressLow;
                unsigned char BaseAddressMiddle;
                unsigned char Type;
                unsigned char LimitHigh;
                unsigned char BaseAddressHigh;
        };
        class TSSEntry {
            public:
                unsigned short LimitLow;
                unsigned short BaseAddressLow;
                unsigned char BaseAddressMiddleLow;
                unsigned char Type;
                unsigned char LimitHigh;
                unsigned char BaseAddressMiddleHigh;
                unsigned int BaseAddressHigh;
                unsigned int Reserved;
        };

        class IDTEntry {
            public:
                unsigned short BaseAddressLow;
                unsigned short Selector;
                unsigned char IST;
                unsigned char Type;
                unsigned short BaseAddressMiddle;
                unsigned int BaseAddressHigh;
                unsigned int Reserved;
        };

        struct Pointer {
            unsigned short Limit;
            unsigned long Base;
        };

        class TSS {
            public:
                unsigned int Reserved1;
                unsigned long RSP[3];
                unsigned long Reserved2;
                unsigned long IST[7];
                unsigned long NoUse3;
                unsigned short Reserved4;
                unsigned short IOMap;
        };

        void Initialize(void);
        void InitGDT(void);
        void InitIDT(void);
        void InitTSS(TSS *Tss);
        void SendToEOI(int InterruptNumber);
        void SetGDTEntry(int Index , unsigned int BaseAddress , unsigned int Limit , unsigned char FlagsHigh , unsigned char FlagsLow , unsigned char Type);
        void SetIDTEntry(int Index , unsigned long BaseAddress , unsigned short Selector , unsigned char Type);
        void SetTSSEntry(TSSEntry *Tss , unsigned long BaseAddress , unsigned int Limit , unsigned char FlagsHigh , unsigned char FlagsLow , unsigned char Type);
    }
}

#endif