memcmp:
; bool memcmp(word *Destination , word *Source , word Count);
	push bp
	mov bp , sp
	pusha
	mov si , word[bp+4]
	mov di , word[bp+6]
	mov cx , word[bp+8]

	.LOOP:
		mov al , byte[si]
		mov bl , byte[di]
		cmp al , bl
		jne .NOTEQUAL
		add si , 1
		add di , 1

		loop .LOOP
	popa
	mov sp , bp
	pop bp
	mov ax , 0x01
	ret

	.NOTEQUAL:
		popa
		mov sp , bp
		pop bp
		mov ax , 0x00
		ret

DebugPrintString:
; void DebugPrintString(word *String);
	push bp
	mov bp , sp
	pusha

	mov si , word[bp+4]
	.LOOP:
		mov al , byte[si]
		cmp al , 0x00
		je .END
		mov ah , 0x0E
		int 0x10
		add si , 1
		jmp .LOOP
	.END:
		popa
		mov sp , bp
		pop bp
		ret 