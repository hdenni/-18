TITLE AddOneToN           (HW5_20141597.asm)

INCLUDE Irvine32.inc
.data
INCLUDE HW5_include.inc

.code
main PROC
	mov esi, 0
	mov al, 31h
	mov ebx, 0
	mov ecx, 31
	compare:
		cmp al, stringEx1[esi]
		je jump
		inc ebx
	jump : 
		inc esi
	loop compare
	mov ecx, 31
	sub ecx, ebx
	mov eax, 0
	call AddOneToN
	
	call WriteDec
	call Crlf
	exit
main ENDP
AddOneToN PROC
	cmp ecx, 0
	je finish
	add eax, ecx
	dec ecx
	call AddOneToN
	finish :
		ret
AddOneToN ENDP
END main