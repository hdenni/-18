TITLE HW3	(HW3_20141597.asm)
INCLUDE Irvine32.inc
.data
INCLUDE HW3_include.inc
.code
main PROC
	mov eax, a1		; eax=a1
	add eax, b1		; eax=a1+b1
	mov ebx, eax	; ebx=a2
	add ebx, a1		; ebx=a2+a1
	sub ebx, b1
	sub ebx, b1
	sub ebx, b1		; ebx=a2+a1-3b1
					; eax=a2 ebx=b2

	mov ecx, eax
	add ecx, ebx	; ecx=a3=a2+b2
	add eax, ecx
	sub eax, ebx
	sub eax, ebx
	sub eax, ebx	; eax=b3 ebx=b2 ecx=a3

	mov ebx, ecx
	add ebx, eax	; ebx=a4
	add ecx, ebx	; ecx=a3+a4
	sub ecx, eax
	sub ecx, eax
	sub ecx, eax	; eax=b3 ebx=a4 ecx=b4

	mov eax, ebx
	add eax, ecx	; eax=a5
	add ebx, eax
	sub ebx, ecx
	sub ebx, ecx
	sub ebx, ecx	; eax=a5 ebx=b5 ecx=b4

	mov ecx, eax
	add ecx, ebx	; ecx=a6
	add eax, ecx
	sub eax, ebx
	sub eax, ebx
	sub eax, ebx	; eax=b6 ebx=b5 ecx=a6

	mov ebx, ecx
	add ebx, eax	; ebx=a7
	add ecx, ebx
	sub ecx, eax
	sub ecx, eax
	sub ecx, eax	; eax=b6 ebx=a7 ecx=b7

	mov eax, ebx
	mov ebx, ecx

	call DumpRegs
	exit
main ENDP
END main


