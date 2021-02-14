#include <sys/DescriptorTables.hpp>
#include <sys/Hardware.hpp>
#include <sys/ISR.hpp>
#include <TextScreen.hpp>
#include <string.hpp>
#include <sys/Task.hpp>

using namespace System;
using namespace System::DescriptorTables;

static DescriptorTables::GDTEntry GDT[4];
static DescriptorTables::IDTEntry IDT[256];

extern "C" void IRQ0_Timer(void);
extern "C" void IRQ12_Mouse(void);
extern "C" void IRQ1_Keyboard(void);

extern "C" void IRQ0_ASM_Timer(void);
extern "C" void IRQ1_ASM_Keyboard(void);
extern "C" void IRQ12_ASM_Mouse(void);

using namespace System;


void DescriptorTables::Initialize(void) {
    Hardware::WritePort(0x20 , 0x11);
    Hardware::WritePort(0x21 , 0x20);
    Hardware::WritePort(0x21 , 0x04);
    Hardware::WritePort(0x21 , 0x01);

    Hardware::WritePort(0xA0 , 0x11);
    Hardware::WritePort(0xA1 , 0x28);
    Hardware::WritePort(0xA1 , 0x02);
    Hardware::WritePort(0xA1 , 0x01);

    Hardware::WritePort(0x21 , 0x00);
    Hardware::WritePort(0xA1 , 0x00);

    InitIDT();
    InitGDT();
    __asm__ ("sti");
}

void DescriptorTables::InitGDT(void) {
    TSS *Tss = (TSS*)TSS_STARTADDRESS;
    Pointer GDTInfo;
    GDTInfo.Limit = ((sizeof(GDTEntry)*3)+(sizeof(TSSEntry)))-1;
    GDTInfo.Base = (unsigned long)GDT;
    SetGDTEntry(0 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00);
    SetGDTEntry(1 , 0x00 , 0xFFFFF , 0xA0 , 0x9A , 0x0A);
    SetGDTEntry(2 , 0x00 , 0xFFFFF , 0xA0 , 0x92 , 0x02);
    SetTSSEntry((TSSEntry*)(&(GDT[3])) , (unsigned long)Tss , sizeof(TSS)-1 , 0x80 , 0x80 , 0x09);
    InitTSS(Tss);

    __asm__ ("mov rax , %0"::"r"(&GDTInfo));
    __asm__ ("lgdt [rax]");
    __asm__ ("mov rax , 0x00");

    __asm__ ("mov di , 0x18");
    __asm__ ("ltr di");
}

void DescriptorTables::InitIDT(void) {
    int i;
    Pointer IDTInfo;
    IDTInfo.Limit = (sizeof(IDTEntry)*256)-1;
    IDTInfo.Base = (unsigned long)(IDT);
    for(i = 0; i < 0xFF; i++) {
        SetIDTEntry(i , 0x00 , 0x00 , 0x00);
    }
    SetIDTEntry(0 , (unsigned long)ISR0_DividedByZero , 0x08 , 0x8E);
    SetIDTEntry(1 , (unsigned long)ISR1_Reserved , 0x08 , 0x8E);
    SetIDTEntry(2 , (unsigned long)ISR2_NMI , 0x08 , 0x8E);
    SetIDTEntry(3 , (unsigned long)ISR3_BreakPoint , 0x08 , 0x8E);
    SetIDTEntry(4 , (unsigned long)ISR4_Overflow , 0x08 , 0x8E);
    SetIDTEntry(5 , (unsigned long)ISR5_BoundsRangeExceeded , 0x08 , 0x8E);
    SetIDTEntry(6 , (unsigned long)ISR6_InvalidOpcode , 0x08 , 0x8E);
    SetIDTEntry(7 , (unsigned long)ISR7_DeviceNotAvailable , 0x08 , 0x8E);
    SetIDTEntry(8 , (unsigned long)ISR8_DoubleFault , 0x08 , 0x8E);
    SetIDTEntry(9 , (unsigned long)ISR9_CoprocessorSegmentOverrun , 0x08 , 0x8E);
    SetIDTEntry(10 , (unsigned long)ISR10_InvalidTSS , 0x08 , 0x8E);
    SetIDTEntry(11 , (unsigned long)ISR11_SegmentNotPresent , 0x08 , 0x8E);
    SetIDTEntry(12 , (unsigned long)ISR12_StackSegmentFault , 0x08 , 0x8E);
    SetIDTEntry(13 , (unsigned long)ISR13_GeneralProtectionFault , 0x08 , 0x8E);
    SetIDTEntry(14 , (unsigned long)ISR14_PageFault , 0x08 , 0x8E);
    SetIDTEntry(15 , (unsigned long)ISR15_Reserved , 0x08 , 0x8E);
    SetIDTEntry(16 , (unsigned long)ISR16_FPUError , 0x08 , 0x8E);
    SetIDTEntry(17 , (unsigned long)ISR17_AlignmentCheck , 0x08 , 0x8E);
    SetIDTEntry(18 , (unsigned long)ISR18_MachineCheck , 0x08 , 0x8E);
    SetIDTEntry(19 , (unsigned long)ISR19_SMIDFloatingPointException , 0x08 , 0x8E);
    SetIDTEntry(20 , (unsigned long)ISR20_Reserved , 0x08 , 0x8E);
    SetIDTEntry(21 , (unsigned long)ISR21_Reserved , 0x08 , 0x8E);
    SetIDTEntry(22 , (unsigned long)ISR22_Reserved , 0x08 , 0x8E);
    SetIDTEntry(23 , (unsigned long)ISR23_Reserved , 0x08 , 0x8E);
    SetIDTEntry(24 , (unsigned long)ISR24_Reserved , 0x08 , 0x8E);
    SetIDTEntry(25 , (unsigned long)ISR25_Reserved , 0x08 , 0x8E);
    SetIDTEntry(26 , (unsigned long)ISR26_Reserved , 0x08 , 0x8E);
    SetIDTEntry(27 , (unsigned long)ISR27_Reserved , 0x08 , 0x8E);
    SetIDTEntry(28 , (unsigned long)ISR28_Reserved , 0x08 , 0x8E);
    SetIDTEntry(29 , (unsigned long)ISR29_Reserved , 0x08 , 0x8E);
    SetIDTEntry(30 , (unsigned long)ISR30_Reserved , 0x08 , 0x8E);
    SetIDTEntry(31 , (unsigned long)ISR31_Reserved , 0x08 , 0x8E);

    SetIDTEntry(32 , (unsigned long)IRQ0_ASM_Timer , 0x08 , 0x8E);
    SetIDTEntry(33 , (unsigned long)IRQ1_ASM_Keyboard , 0x08 , 0x8E);
    SetIDTEntry(44 , (unsigned long)IRQ12_ASM_Mouse , 0x08 , 0x8E);

    __asm__ ("mov rax , %0"::"r"(&IDTInfo));
    __asm__ ("lidt [rax]");
}

void DescriptorTables::InitTSS(TSS *Tss) {
    memset(Tss , 0 , sizeof(TSS));
    Tss->IST[0] = IST_STARTADDRESS+IST_SIZE;
    Tss->IOMap = 0xFFFF;
}

void DescriptorTables::SetGDTEntry(int Index , unsigned int BaseAddress , unsigned int Limit , unsigned char FlagsHigh , unsigned char FlagsLow , unsigned char Type) {
    GDT[Index].LimitLow = Limit & 0xFFFF;
    GDT[Index].LimitHigh = ((Limit >> 16) & 0xFF)|FlagsHigh;
    GDT[Index].BaseAddressLow = BaseAddress & 0xFFFF;
    GDT[Index].BaseAddressMiddle = (BaseAddress >> 16) & 0xFF; 
    GDT[Index].BaseAddressHigh = (BaseAddress >> 24) & 0xFF;
    GDT[Index].Type = FlagsLow|Type;
}

void DescriptorTables::SetIDTEntry(int Index , unsigned long BaseAddress , unsigned short Selector , unsigned char Type) {
    IDT[Index].BaseAddressLow = (unsigned long)(BaseAddress & 0xFFFF);
    IDT[Index].BaseAddressMiddle = (unsigned long)(BaseAddress >> 16) & 0xFFFF;
    IDT[Index].BaseAddressHigh = (unsigned long)(BaseAddress >> 32);
    IDT[Index].Selector = Selector;
    IDT[Index].Type = Type;
    IDT[Index].IST = 0x01 & 0x3;
    IDT[Index].Reserved = 0;
}

void DescriptorTables::SetTSSEntry(TSSEntry *Tss , unsigned long BaseAddress , unsigned int Limit , unsigned char FlagsHigh , unsigned char FlagsLow , unsigned char Type) {
    Tss->BaseAddressLow = BaseAddress & 0xFFFF;
    Tss->BaseAddressMiddleLow = (BaseAddress >> 16) & 0xFF;
    Tss->BaseAddressMiddleHigh = (BaseAddress >> 24) & 0xFF;
    Tss->BaseAddressHigh = BaseAddress >> 32;
    Tss->LimitLow = Limit & 0xFFFF;
    Tss->LimitHigh = ((Limit >> 16) & 0xFF)|FlagsHigh;
    Tss->Type = FlagsLow|Type;
    Tss->Reserved = 0;
}

void DescriptorTables::SendToEOI(int InterruptNumber) {
    if(InterruptNumber >= 40) {
        Hardware::WritePort(0xA0 , 0x20);
    }
    Hardware::WritePort(0x20 , 0x20);
}


extern "C" void IRQ0_Timer(void) {
    DescriptorTables::SendToEOI(32);
    Task::SwitchTaskInInterrupt();
    Hardware::Timer::Interrupt();
}

extern "C" void IRQ1_Keyboard(void) {
    unsigned char KeyCode;
    DescriptorTables::SendToEOI(33);
    if(Hardware::ReadPort(0x64) & 0x01) {
        KeyCode = Hardware::ReadPort(0x60);
        Hardware::Keyboard::ProcessAndPutToQueue(KeyCode);
    }
}

extern "C" void IRQ12_Mouse(void) {
    DescriptorTables::SendToEOI(44);
    if(Hardware::ReadPort(0x64) & 0x01) {
        Hardware::Mouse::ProcessAndPutToQueue(Hardware::ReadPort(0x60));
    }
}