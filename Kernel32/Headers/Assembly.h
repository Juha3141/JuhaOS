#ifndef _ASSEMBLY_H_
#define _ASSEMBLY_H_

#include <Types.h>

void WritePort(unsigned short Port , unsigned char Data);
unsigned char ReadPotr(unsigned short Port);
void SwitchTo64BitAndJump(void);
bool Check64BitSupported(void);

#endif