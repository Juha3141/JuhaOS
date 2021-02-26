; BootLoader.asm
; Description : 부트로더 코드
; (C) 2020. All rights reserved by Juha3141.
; 원본 파일

[BITS 16]	; 16비트 모드 코드를 사용한다.

	db 0xEB , 0x3C , 0x90   ; 점프코드, START로 점프한다.
							; 이것은 뒤에 나오는 BPB를 CPU가 코드로 인식하지 않기 위해서이다.

; ========== BPB(BIOS PARAMETER BLOCK) ==========
OEMID: db "JUHAOS  "        ; OEM Label, 8바이트
BytesPerSector: dw 0x200	; 한 섹터가 차지하는 바이트의 수, 512바이트
SectorPerCluster: db 0x01	; 한 클러스터가 차지하는 섹터 수
ReservedSector: dw 0x01		; 예약된 섹터, 부트로더가 하나 차지하기 때문에 1이 된다.
FATCount: db 0x02       	; FAT가 몇개 있는지이다, FAT12/FAT16은 2개가 있다.
RootDirectoryCount: dw 224	; 루트 디렉토리 개수
TotalSector16: dw 2880		; 총 섹터 수
MediaType: db 0xF0			; 미디어 종류, 플로피 디스크를 제외하면 모두 0xF8이지만,
							; 여기서는 플로피 디스크를 사용할 것이기 때문에 0xF0이다.
FATSize16: dw 0x09  		; FAT 하나가 차지하는 섹터 수, 9개를 차지한다.
SectorPerTrack: dw 18		; 한 트랙 당 차지하는 섹터 수
NumberOfHeads: dw 2			; 헤드의 개수
HiddenSectors: dd 0x00    	; 숨겨진 섹터 수
TotalSector32: dd 0x00 		; 총 섹터 수, TotalSector16에서 바이트 제한때문에 다 입력하지 못했다면 
							; 이 필드를 사용한다.
DriveNumberInt0x13: db 0x00 ; int 0x13 바이오스 인터럽트에 사용할 드라이브 번호이다
							; 플로피 디스크 = 0x00, CDROM = 0x80
Reserved: db 0x00           ; 예약된 필드이다
Signature: db 29			; 시그니처
SerialNumber: dd 0x00 		; 시리얼 넘버는 공백으로 두었다
VolumeLabel: db "DOCSOSBOOT "	; 볼륨 라벨
FileSystemType: db "FAT12   "	; 파일 시스템, FAT12를 사용한다

; ========== START(ENTRY) ==========

START:				; 부트로더의 시작점
	mov ax , 0x7C0	; ds세그먼트(데이터 세그먼트)를 0x07C0으로 설정해서
					; 사용하는 주소값이 모두 0x7C0을 세그먼트로 해서 사용할 수 있도록 한다.
					; 이것은 ORG 0x7C00으로 데처할 수 있지만 BPB 때문에 그러지 못하게 되었다.
	mov ds , ax
	mov ax , 0x00	; 스택 세그먼트를 설정한다, 스택의 실제 주소는 ss:sp이기 때문에 스택 세그먼트를 
					; 초기화 하지 않는다면 스택 기능을 사용하기 어렵다.
	mov ss , ax
	mov sp , 0xFFFF	; sp와 bp를 모두 0xFFFF로 초기화한다, 이 주소는 
	mov bp , 0xFFFF	; 나중에 32비트 커널에서도 사용될 예정이다.

	mov ax , 0x00	; al레지스터를 사용하기 위해서 ax레지스터를 0으로 설정한다
	mov bx , 0x00   ; bl레지스터를 사용할 것이기에 bx레지스터도 0으로 초기화한다
					; 참고로 초기화할 때 대부분은 xor bx , bx를 사용한다
	cld  			; 리로드 역할을 한다
	mov al , byte[ReservedSector]			; 데이터 영역이 어떤 섹터에 있는지 확인하는 연산이다
	mov byte[RootDirectoryLocation] , al	; 데이터 영역은 FAT영역 바로 다음에 위치하기 때문에
	mov al , byte[FATCount]					; (FATSize16*FATCount)+ReservedSector가 된다
	mov bl , byte[FATSize16]
	imul ax , bx							; FATSize16과 FATCount를 곱한다
	add byte[RootDirectoryLocation] , al	; 루트 디렉토리 주소를 저장하는 변수에 결과값을 더한다.
											; 결과값은 0x13, 19가 된다. 실제로 19번 섹터에 가보면 루트 
											; 디렉토리가 존재한다.
	mov ax , word[RootDirectoryCount]		; 루트 디렉토리를 읽을 섹터가 몇개인지 계산하는 코드이다(61~65줄)
	mov bx , 0x20							; 루트 디렉토리는 하나 당 0x20바이트(32바이트)를 차지하기 때문에
	imul ax , bx							; (BPB에 들어있는 루트 디렉토리 개수)*32/512(Bytes per Sector)를 계산해서 
	mov bx , word[BytesPerSector]			; ax레지스터에 저장하는 코드이다
	div bx									; ax레지스타와 bx레지스터를 나눈 후 몫은 ax레지스터에 저장하고 나머지는 dx레지스터에 저장한다.

    push word[DriveNumberInt0x13]	 ; 드라이브 번호 		| 루트 디렉토리를 읽은 후 0x60:0x00(0x600)번지에 저장하는 코드이다
    push 0x00						 ; 메모리 오프셋	    | 전에 만들어둔 ReadSectorCHS함수를 사용했다
    push 0x60						 ; 메모리 주소
    push ax							 ; 읽을 섹터 수
    push word[RootDirectoryLocation] ; 읽을 섹터 번호
    call ReadSectorCHS				 ; 섹터 읽는 함수 실행
    add sp , 10			; 스택에서 넣은 인자값을 제거한다
    cmp ax , 0x00		; ax가 0이면 에러가 난 것이기 때문에
    je HandleError 		; 에러 핸들링한다
; ========== DS세그먼트 교체 ==========
	mov bx , 0x00		; 여기서부터는 메모리에 직접 접근해서 파일의 위치가 어디인지 확인하는 부분이기 때문에
	mov ds , bx			; 데이터 세그먼트를 0x07C0에서 0x00으로 바꿔준다. 그렇게 하면 0x1000이라고 정한 주소다
						; 0x8C00(0x7C0:0x1000 = 0x8C00, 저도 잘 모르지만 아마도 이거일거에요)
						; 으로 인식되는것이 아니라 아니라 0x1000(0x0000:0x1000 = 0x1000)으로 인식되기 때문이다.
	mov si , 0x600		; si 레지스터를 사용해서 루트 디렉터리를 읽을 것이기 때문에 전에 루트 디렉토리를 저장했던 주소(0x600, 69줄 참고)
						; 로 설정한다.
	mov cx , 0x00		; 지금까지 몇개의 루트 디렉토리를 읽었는지 확인하는 용도이다.
	.FINDFILE:
		cmp cx , word[RootDirectoryCount]	; 만일 모든 루트 디렉토리를 읽어도 원하는 파일을 찾지 못했다면
		je .HandleError         			; 파일이 존재하지 않는 것이기 때문에 에러로 넘어간다.
; ========== 파일이름 검사 ==========
	    push 11					; 파일 이름(파일 이름 8바이트+확장자 3바이트 해서 11바이트)을 검사한다
	    						; 파일 이름은 루트 디렉토리의 0번째 오프셋에 위치하기 때문에 추가로 오프셋을 가지 않고
	    						; si 그대로를 검사한다(si를 32만큼 증가하면서 검사할 것임).
	    push FileName+0x7C00	; 전에 ds세그먼트를 0x00으로 설정해 놓았기 때문에 FileName변수의 위치를 다른 곳으로 인식한다,
	    				; 그렇기 때문에 0x7C00(부트로더의 시작점)을 더해서 원래 위치로 인식할 수 있도록 해준다.
	    push si 		; si레지스터와 FileName변수를 각각 11바이트만큼 검사한다.
	    call memcmp		; 메모리 검사 함수 실행
	    add sp , 6		; 함수 인자 제거
	    cmp ax , 0x01	; 만일 이름이 같다면(ax = 0x01)
	    je .FOUND		; 찾은 것이기 때문에 다음 단계로 진행한다 

		add si , 0x20	; si를 루트 디렉토리의 크기(32)만큼  
		add cx , 0x01	; 몇개의 루트 디렉토리를 읽었는지 확인하기 위해 cx레지스터를 1 증가시킨다.
		jmp .FINDFILE	; 다시 반복한다.
	.FOUND:					; 파일 찾음
		add si , 0x1A		; 파일의 클러스터 위치는 루트 디렉토리 상 0x1A에 존재하기 때문에 
		mov ax , word[si]	; si를 0x1A 증가해서 클러스터 위치 정보를 ax에 저장한다.
		add ax , 31		; 많은 검색 끝에 파일 위치는 클러스터 위치+31에 있다는 것을 알게 되었다
						; 출처:mikeos 부트로더 195번줄 참고

		mov bx , 0x7C0	; 이제 어떤 섹터에 있는지 알았기 때문에 데이터 세그먼트를 0x7C0으로 다시 설정한다
		mov ds , bx		; 섹터 읽는 함수가 잘 작동할 수 있게 하기 위해서이다.

	    push word[DriveNumberInt0x13]		; 드라이브 번호
	    push 0x00				; 메모리 오프셋
	    push 0x400				; 원래는 0x1000으로 하려 그래지만 루트 디렉토리를 고려
	    						; (루트 디렉토리 14섹터 = 0x1C00, 0x1C00+0x600(루트 디렉토리 시작 위치) = 0x2200)해서 가장 적당한 0x4000으로 정했다.
	    push word[KernelSize]	; 최대 8KB(148줄 참고)
	    push ax					; 몇번째 섹터에 있는지를 입력(105줄 참고)
	    call ReadSectorCHS		; 섹터 읽기 함수 실행
	    add sp , 10		; 함수 인자 제거 
		cmp ax , 0x00	; 만일 실패했다면 (ax = 0x00)
		je HandleError 	; 에러 핸들링(.HandleError는 ds세그먼트가 0일때 사용하고
						; HandleError는 ds세그먼트가 0x7C0일때 사용한다)

		jmp 0x400:0x00	; 16비트 커널로 점프

	.HandleError:
		mov ax , 0xAFAF			; 디버그용
		push DebugError+0x7C00	; 에러 메세지 출력, ds가 0x00이기 때문에 0x7C00을 더한다.
		call DebugPrintString	; 문자열 출력 함수 실행
		add sp , 2				; 함수 인자 제거
		jmp $		; 에러가 났기 때문에 무한루프를 돈다

HandleError:
	mov ax , 0xFAFA			; 디버그용
	push DebugError 		; 에러 메세지 출력
	call DebugPrintString	; 문자열 출력 함수 실행
	add sp , 2				; 함수 인자 제거
	jmp $		; 에러가 났기 때문에 무한루프를 돈다

; ========== ReadSectorCHS함수에서 사용할 변수들 ==========
Sector: db 0x00 ; 섹터
Head: db 0x00  	; 헤드
Track: db 0x00  ; 트랙(실린더라고도 한다)

%include 'Functions.inc'	; 함수들이 들어있는 파일을 포함한다.

DebugError: db "Critical Error" , 0x00 	; 에러메세지
RootDirectoryLocation: dw 0x00 	; 루트 디렉토리 주소를 저장할 변수
ClusterLocation: dw 0x00 	; 파일의 클러스터 주소를 저장할 변수
KernelSize: dw 16	 		; 커널 크기

FileName: db "KERNEL16BIN"	; 찾을 커널 파일의 이름

times (510-($-$$)) db 0x00	; 510바이트로 맞춤
dw 0xAA55		; 부트로더 시그니처