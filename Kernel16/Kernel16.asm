[ORG 0x4000]
[BITS 16]

START:
	mov ax , 0x00
	mov ds , ax
	mov ss , ax
	mov sp , 0xFFFF
	mov bp , 0xFFFF
	
	push 0x00
	push 0x1000
	push 4
	push Kernel32FileName
	call FindFile

	push 0x00
	push 0x800
	call CheckMemory

	lgdt [GDTInfo]

	cli
	mov eax , cr0
	or eax , 0x01
	mov cr0 , eax
	jmp 0x08:Kernel32

FindFile:
; bool FindFile(word *FileName , byte SectorCount , word Address , word MemoryOffset);
	push bp
	mov bp , sp
	pusha

	mov di , word[bp+4]
	mov ax , word[bp+6]
	mov word[SectorCountToRead] , ax
	mov ax , word[bp+8]
	mov es , ax
	mov bx , word[bp+10]

	mov si , 0x600
	mov cx , 0x00

	.LOOP:
		cmp cx , word[RootDirectoryCount]
		je .ERROR
	    push 11
	    push di
	    push si
	    call memcmp
	    add sp , 6
	    cmp ax , 0x01
	    je .FOUND

		add si , 0x20
		add cx , 0x01
		jmp .LOOP
	.FOUND:
		add si , 0x1A
		mov ax , word[si]
		add ax , 31

	    push word[DriveNumber]
	    push 0x00
	    push es
	    push word[SectorCountToRead]
	    push ax
	    call ReadSector
	    add sp , 10
		cmp ax , 0x00
		je .ERROR

		popa
		mov sp , bp
		pop bp
		mov ax , 0x01
		ret

	.ERROR:
		popa
		mov sp , bp
		pop bp
		mov ax , 0x00
		ret

ReadSector: 
; bool ReadSector(byte Sector , byte SectorCountToRead , word Address , word MemoryOffset , byte DriveNumber);
	push bp
	mov bp , sp
	pusha
	mov ax , 0x00
	mov bx , 0x00
	mov cx , 0x00
	mov dx , 0x00

	mov ax , [bp+4]
	mov bx , 18
	div bx
	add dl , 1
	mov byte[Sector] , dl
	mov dx , 0x00
	mov bx , 0x02
	div bx

	mov byte[Head] , dl
	mov ax , word[bp+4]
	mov bl , 18*2
	div bl
	
	mov byte[Track] , al

	mov ax , [bp+8]
	mov es , ax
	mov bx , [bp+10]

	mov ah , 0x02
	mov cl , byte[Sector]
	mov dh , byte[Head]
	mov ch , byte[Track]
	mov dl , byte[bp+12]
    mov al , byte[bp+6]
    int 0x13
    jc .FAIL
    popa
    mov sp , bp
    pop bp
    mov ax , 0x01
    ret

    .FAIL:
	    popa
	    mov sp , bp
	    pop bp
    	mov ax , 0x00
    	ret

CheckMemory:
	push bp
	mov bp , sp
	pusha

	mov ax , word[bp+4]
	mov es , ax
	mov di , word[bp+6]
	
	mov ebx , 0x00
	
	.LOOP:
		mov eax , 0xE820
		mov edx , 0x534D4150
		mov ecx , 24
		int 0x15

		cmp ebx , 0x00
		je .CONTINUE
		
		add di , 24
		jmp .LOOP
	
	.CONTINUE:
		popa
		mov sp , bp
		pop bp
		ret

GDTInfo:
    dw GDT_END-GDT-1
    dd GDT

GDT:
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
        db 0b10011010
        db 0b11001111
        db 0x00
    DataSegment:
        dw 0xFFFF
        dw 0x00
        db 0x00
        db 0b10010010
        db 0b11001111
        db 0x00
GDT_END:

%include 'Functions.inc'

Sector: db 0x00
Head: db 0x00
Track: db 0x00

FileSize: dd 0x00
SectorLocation: dw 0x00
DriveNumber: db 0x00
SectorCountToRead: dw 0x00
ClusterLocation: dw 0x00
Kernel32FileName: db "KERNEL32BIN"
RootDirectoryCount: dw 224

[BITS 32]

Kernel32:
    mov ax , 0x10
    mov es , ax
    mov ds , ax
    mov ss , ax
    mov fs , ax
    mov gs , ax

    mov esp , 0xFFFF
    mov ebp , 0xFFFF

	jmp 0x10000
	
	jmp $

times (1024-($-$$)) db 0x00