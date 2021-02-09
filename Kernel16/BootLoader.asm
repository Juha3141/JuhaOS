[BITS 16]

	db 0xEB , 0x3C , 0x90

OEMID: db "JUHAOS  "
BytesPerSector: dw 0x200
SectorPerCluster: db 0x01
ReservedSector: dw 0x01
FATCount: db 0x02
RootDirectoryCount: dw 224
TotalSector16: dw 2880
MediaType: db 0xF0
FATSize16: dw 0x09
SectorPerTrack: dw 18
NumberOfHeads: dw 2
HiddenSectors: dd 0x00
TotalSector32: dd 0x00
DriveNumberInt0x13: db 0x00
Reserved: db 0x00
Signature: db 29
SerialNumber: dd 0x00
VolumeLabel: db "JUHAOSBOOT "
FileSystemType: db "FAT12   "


START:
	mov ax , 0x7C0
	mov ds , ax
	mov ax , 0x00
	mov ss , ax
	mov sp , 0xFFFF 
	mov bp , 0xFFFF

	mov ax , 0x00
	mov bx , 0x00
	mov cx , 0x00
	cld

	mov al , byte[ReservedSector]
	mov byte[RootDirectoryLocation] , al
	mov al , byte[FATCount]
	mov bl , byte[FATSize16]
	imul ax , bx
	add byte[RootDirectoryLocation] , al
	mov ax , word[RootDirectoryCount]
	mov bx , 0x20
	imul ax , bx
	mov bx , word[BytesPerSector]
	div bx

    push word[DriveNumberInt0x13]
    push 0x00
    push 0x60
    push ax
    push word[RootDirectoryLocation]
    call ReadSectorCHS
    add sp , 10
    cmp ax , 0x00
    je HandleError
	mov bx , 0x00
	mov ds , bx
	mov si , 0x600
	mov cx , 0x00
	.FINDFILE:
		cmp cx , word[RootDirectoryCount]
		je .HandleError
	    push 11
	    push FileName+0x7C00
	    push si
	    call memcmp
	    add sp , 6
	    cmp ax , 0x01
	    je .FOUND

		add si , 0x20
		add cx , 0x01
		jmp .FINDFILE
	.FOUND:
		add si , 0x1A
		mov ax , word[si]
		add ax , 31
		mov bx , 0x7C0
		mov ds , bx

	    push word[DriveNumberInt0x13]
	    push 0x00
	    push 0x400
	    push word[KernelSize]
	    push ax
	    call ReadSectorCHS
	    add sp , 10
		cmp ax , 0x00
		je HandleError

		jmp 0x400:0x00

		jmp $

	.HandleError:
		mov ax , 0xAFAF
		push DebugError+0x7C00
		call DebugPrintString
		add sp , 2
		jmp $

HandleError:
	mov ax , 0xFAFA
	push DebugError 
	call DebugPrintString
	add sp , 2
	jmp $

ReadSectorCHS: 
; bool ReadSectorCHS(byte Sector , byte SectorCountToRead , word Address , word MemoryOffset , byte DriveNumber);	push bp
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
	mov bx , 0x00
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

Sector: db 0x00
Head: db 0x00
Track: db 0x00

%include 'Functions.inc'

DebugError: db "Critical Error" , 0x00
RootDirectoryLocation: dw 0x00
ClusterLocation: dw 0x00
KernelSize: dw 16

FileName: db "KERNEL16BIN"

times (510-($-$$)) db 0x00
dw 0xAA55