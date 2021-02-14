#include <sys/ISR.hpp>
#include <TextScreen.hpp>
#include <sys/Task.hpp>
#include <string.hpp>

using namespace System;
using namespace System::DescriptorTables;
using namespace System::Hardware;

static void HandleError(const char *ErrorMessage) {
    int X;
    int Y;
    char *ContextAddress = (char*)IST_STARTADDRESS+IST_SIZE-sizeof(Task::REGISTERS);
    Task::REGISTERS Registers;
    memcpy(&(Registers) , ContextAddress , sizeof(Task::REGISTERS)); 
    TextScreen::ClearScreen(0x04);
    TextScreen::printf("Something was really wrong in Kernel system, so The OS has to be halted\n");
    TextScreen::printf("Here's some information about error:\n\n");
    TextScreen::printf("Error message:%s\n" , ErrorMessage);
    TextScreen::printf("-----------------------------------------------------------------\n");

    TextScreen::printf("RAX    : 0x%X" , Registers.RAX);
    TextScreen::GetCursor(&(X) , &(Y));
    TextScreen::MoveCursor(18 , Y);
    TextScreen::printf(" | RBX    : 0x%X" , Registers.RBX);
    TextScreen::MoveCursor(38 , Y);
    TextScreen::printf(" | RCX    : 0x%X\n" , Registers.RCX);

    TextScreen::printf("RDX    : 0x%X" , Registers.RDX);
    TextScreen::GetCursor(&(X) , &(Y));
    TextScreen::MoveCursor(18 , Y);
    TextScreen::printf(" | RSI    : 0x%X" , Registers.RSI);
    TextScreen::MoveCursor(38 , Y);
    TextScreen::printf(" | RDI    : 0x%X\n" , Registers.RDI);

    TextScreen::printf("RSP    : 0x%X" , Registers.RSP);
    TextScreen::GetCursor(&(X) , &(Y));
    TextScreen::MoveCursor(18 , Y);
    TextScreen::printf(" | RBP    : 0x%X" , Registers.RBP);
    TextScreen::MoveCursor(38 , Y);
    TextScreen::printf(" | R8     : 0x%X\n" , Registers.R8);

    TextScreen::printf("R9     : 0x%X" , Registers.R9);
    TextScreen::GetCursor(&(X) , &(Y));
    TextScreen::MoveCursor(18 , Y);
    TextScreen::printf(" | R10    : 0x%X" , Registers.R10);
    TextScreen::MoveCursor(38 , Y);
    TextScreen::printf(" | R11    : 0x%X\n" , Registers.R11);

    TextScreen::printf("R12    : 0x%X" , Registers.R12);
    TextScreen::GetCursor(&(X) , &(Y));
    TextScreen::MoveCursor(18 , Y);
    TextScreen::printf(" | R13    : 0x%X" , Registers.R13);
    TextScreen::MoveCursor(38 , Y);
    TextScreen::printf(" | R14    : 0x%X\n" , Registers.R14);

    TextScreen::printf("R15    : 0x%X" , Registers.R15);
    TextScreen::GetCursor(&(X) , &(Y));
    TextScreen::MoveCursor(18 , Y);
    TextScreen::printf(" | RIP    : 0x%X" , Registers.RIP);
    TextScreen::MoveCursor(38 , Y);
    TextScreen::printf(" | RFlags : 0x%X\n" , Registers.RFlags);

    TextScreen::printf("CS     : 0x%X" , Registers.CS);
    TextScreen::GetCursor(&(X) , &(Y));
    TextScreen::MoveCursor(18 , Y);
    TextScreen::printf(" | DS     : 0x%X" , Registers.DS);
    TextScreen::MoveCursor(38 , Y);
    TextScreen::printf(" | SS     : 0x%X\n" , Registers.SS);

    TextScreen::printf("ES     : 0x%X" , Registers.ES);
    TextScreen::GetCursor(&(X) , &(Y));
    TextScreen::MoveCursor(18 , Y);
    TextScreen::printf(" | FS     : 0x%X" , Registers.FS);
    TextScreen::MoveCursor(38 , Y);
    TextScreen::printf(" | GS     : 0x%X\n" , Registers.GS);
    TextScreen::printf("-----------------------------------------------------------------\n");
    TextScreen::printf("Contact : ianisnumber2027@gmail.com\nWebsite:http://juhaos.surge.sh\n\n");
    TextScreen::EnableCursor(0xFF , 0xFF);
}

void ISR0_DividedByZero(void) {
    SendToEOI(0x00);
    HandleError("Divided by zero");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR1_Reserved(void) {
    SendToEOI(0x01);
    HandleError("Reserved #1");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR2_NMI(void) {
    SendToEOI(0x02);
    HandleError("NMI Interrupt");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR3_BreakPoint(void) {
    SendToEOI(0x03);
    HandleError("NMI Interrupt");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR4_Overflow(void) {
    SendToEOI(0x04);
    HandleError("Overflow(Why did you do that)");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR5_BoundsRangeExceeded(void) {
    SendToEOI(0x05);
    HandleError("Bounds range exceede");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR6_InvalidOpcode(void) {
    SendToEOI(0x06);
    HandleError("Invalid opcode");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR7_DeviceNotAvailable(void) {
    SendToEOI(0x07);
    HandleError("Device not available");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR8_DoubleFault(void) {
    SendToEOI(0x08);
    HandleError("Double fault");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR9_CoprocessorSegmentOverrun(void) {
    SendToEOI(0x09);
    HandleError("Coprocessor segment overrun");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR10_InvalidTSS(void) {
    SendToEOI(0x0A);
    HandleError("Invalid TSS");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR11_SegmentNotPresent(void) {
    SendToEOI(0x0B);
    HandleError("Segment not present");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR12_StackSegmentFault(void) {
    SendToEOI(0x0C);
    HandleError("Stack segment fault");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR13_GeneralProtectionFault(void) {
    SendToEOI(0x0D);
    HandleError("General protection fault");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR14_PageFault(void) {
    SendToEOI(0x0E);
    HandleError("Page fault");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR15_Reserved(void) {
    SendToEOI(0x0F);
    HandleError("Reserved");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR16_FPUError(void) {
    SendToEOI(0x10);
    HandleError("FPU error");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR17_AlignmentCheck(void) {
    SendToEOI(0x11);
    HandleError("Alignment check");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR18_MachineCheck(void) {
    SendToEOI(0x12);
    HandleError("Machine check");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR19_SMIDFloatingPointException(void) {
    SendToEOI(0x13);
    HandleError("SMID Floating-Point exception");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR20_Reserved(void) {
    SendToEOI(0x14);
    HandleError("Reserved");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR21_Reserved(void) {
    SendToEOI(0x15);
    HandleError("Reserved");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR22_Reserved(void) {
    SendToEOI(0x16);
    HandleError("Reserved");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR23_Reserved(void) {
    SendToEOI(0x17);
    HandleError("Reserved");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR24_Reserved(void) {
    SendToEOI(0x18);
    HandleError("Reserved");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR25_Reserved(void) {
    SendToEOI(0x19);
    HandleError("Reserved");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR26_Reserved(void) {
    SendToEOI(0x1A);
    HandleError("Reserved");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR27_Reserved(void) {
    SendToEOI(0x1B);
    HandleError("Reserved");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR28_Reserved(void) {
    SendToEOI(0x1C);
    HandleError("Reserved");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR29_Reserved(void) {
    SendToEOI(0x1D);
    HandleError("Reserved");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR30_Reserved(void) {
    SendToEOI(0x1E);
    HandleError("Reserved");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ISR31_Reserved(void) {
    SendToEOI(0x1F);
    HandleError("Reserved");
    while(1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}