[BITS 32]

SECTION .text

global Entry
extern Main32

Entry:
    cli 
    
    mov eax , 0xB8000
    mov byte[eax] , 'a'

    jmp $