#ifndef _ISR_H_
#define _ISR_H_

#include <Types.hpp>
#include <sys/DescriptorTables.hpp>
#include <sys/Hardware.hpp>

void ISR0_DividedByZero(void);
void ISR1_Reserved(void);
void ISR2_NMI(void);
void ISR3_BreakPoint(void);
void ISR4_Overflow(void);
void ISR5_BoundsRangeExceeded(void);
void ISR6_InvalidOpcode(void);
void ISR7_DeviceNotAvailable(void);
void ISR8_DoubleFault(void);
void ISR9_CoprocessorSegmentOverrun(void);
void ISR10_InvalidTSS(void);
void ISR11_SegmentNotPresent(void);
void ISR12_StackSegmentFault(void);
void ISR13_GeneralProtectionFault(void);
void ISR14_PageFault(void);
void ISR15_Reserved(void);
void ISR16_FPUError(void);
void ISR17_AlignmentCheck(void);
void ISR18_MachineCheck(void);
void ISR19_SMIDFloatingPointException(void);
void ISR20_Reserved(void);
void ISR21_Reserved(void);
void ISR22_Reserved(void);
void ISR23_Reserved(void);
void ISR24_Reserved(void);
void ISR25_Reserved(void);
void ISR26_Reserved(void);
void ISR27_Reserved(void);
void ISR28_Reserved(void);
void ISR29_Reserved(void);
void ISR30_Reserved(void);
void ISR31_Reserved(void);

#endif