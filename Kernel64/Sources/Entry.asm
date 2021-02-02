[BITS 64]

SECTION .text

global Entry
extern Main

Entry:
    mov ax , 0x10
    mov ds , ax
    mov es , ax
    mov fs , ax
    mov gs , ax
    mov ss , ax

    mov rsp , 0x6FFFFE
    mov rbp , 0x6FFFFE

    call Main

    Halt:
        hlt
        jmp Halt