#ifndef _DESCRIPTORTABLES_H_
#define _DESCRIPTORTABLES_H_

#include <Types.hpp>

#define TSS_STARTADDRESS 0x400000
#define IST_STARTADDRESS 0x700000
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
                unsigned char BaseAddressMiddle1;
                unsigned char Type;
                unsigned char LimitHigh;
                unsigned char BaseAddressMiddle2;
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
                unsigned int NoUse;
        };

        class Pointer {
            public:
                unsigned short Limit;
                unsigned long Base;

                char Padding[6];
        };

        class TSS {
            public:
                unsigned int NoUse1;
                unsigned long RSP[3];
                unsigned long NoUse2;
                unsigned long IST[7];
                unsigned long NoUse3;
                unsigned short NoUse4;
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