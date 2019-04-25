TITLE Sogang Lottery           (HW4_20141597.asm)
INCLUDE Irvine32.inc

.data
str1 BYTE "Sogang Lottery!",0
str2 BYTE "Bye Lottery!",0
lotto DWORD 6 DUP(?)
count DWORD 0
.code
main PROC
	top:
		call Randomize
		mov edx, OFFSET str1
		call WriteString
		call Crlf
		
		mov esi, 0
		mov ecx, 6
		L1 :
			call CreateRandomNumber
			mov lotto[esi], eax
			add esi, 4
		loop L1
		
		; Sorting
		mov ecx, 5
		mov esi, 0
		for6 :
		mov eax, lotto[esi]
		cmp eax, lotto[esi+4]
		jb next6
		xchg eax, lotto[esi+4]
		mov lotto[esi], eax
		next6 : add esi, 4
		loop for6

		mov ecx, 4
		mov esi, 0
		for5 :
		mov eax, lotto[esi]
		cmp eax, lotto[esi+4]
		jb next5
		xchg eax, lotto[esi+4]
		mov lotto[esi], eax
		next5 : add esi, 4
		loop for5

		mov ecx, 3
		mov esi, 0
		for4 :
		mov eax, lotto[esi]
		cmp eax, lotto[esi+4]
		jb next4
		xchg eax, lotto[esi+4]
		mov lotto[esi], eax
		next4 : add esi, 4
		loop for4

		mov ecx, 2
		mov esi, 0
		for3 :
		mov eax, lotto[esi]
		cmp eax, lotto[esi+4]
		jb next3
		xchg eax, lotto[esi+4]
		mov lotto[esi], eax
		next3 : add esi, 4
		loop for3
		
		mov esi, 0
		mov eax, lotto[esi]
		cmp eax, lotto[esi+4]
		jb next
		xchg eax, lotto[esi+4]
		mov lotto[esi], eax
						
		next :
		mov esi, 0
		mov ecx, 6
		L2 :
			mov eax, 500
			call Delay
			mov eax, lotto[esi]
			call Writedec
			call Crlf
			add esi, 4
		loop L2
		
	call ReadChar
	cmp al, 'r'
	je top
	cmp al, 'q'
	je last
	last : 
		mov edx, OFFSET str2
		call WriteString
		call Crlf
		exit
main ENDP

CreateRandomNumber PROC
	mov eax, 45
	call randomrange
	inc eax
	ret
CreateRandomNumber ENDP
END main