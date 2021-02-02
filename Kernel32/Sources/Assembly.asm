[BITS 32]

global ReadPort
global WritePort
global Check64BitSupported
global SwitchTo64BitAndJump

SECTION .text

ReadPort:
    mov edx , [esp+4]
    mov eax , 0x00
    in al , dx
    ret

WritePort:
    mov edx , [esp+4]
    mov al , [esp+8]
    out dx , al
    ret

Check64BitSupported:
    mov eax , 0x80000001
    cpuid
    test edx , 1 << 29
    jz NotSupported
    mov eax , 0x01
    ret
    NotSupported:
        mov eax , 0x00
        ret

SwitchTo64BitAndJump:
    mov eax , cr4
    or eax , 0x20
    mov cr4 , eax

    mov eax , 0x16000
    mov cr3 , eax

    mov ecx , 0xC0000080
    rdmsr

    or eax , 0x0101
    wrmsr
    
    mov eax , cr0
    or eax , 0xE0000000
    xor eax , 0x60000000 
    mov cr0 , eax
    lgdt [LongModeGDTInfo]

    jmp 0x08:0x200000

LongModeGDT:
    NullSegment:
        dw 0x00
        dw 0x00
        db 0x00
        db 0x00
        db 0x00 
        db 0x00
    CodeSegment:
        dw 0xFFFF
        dw 0x00
        db 0x00
        db 0x9A
        db 0xAF
        db 0x00
    DataSegment:
        dw 0xFFFF
        dw 0x00 
        db 0x00
        db 0x92
        db 0xAF
        db 0x00

LongModeGDTInfo:
    dw $-LongModeGDT-1
    dd LongModeGDT