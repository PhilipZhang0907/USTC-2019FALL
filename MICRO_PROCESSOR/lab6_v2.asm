.MODEL SMALL
.386

.DATA
	TEN 			dd 	10.0
	TEMP 			dw 	?
	RESULT 			dd 	?
	INTEGRAL 		dd 	?
	FRACTIONAL 		dd 	?
	SIGN 			db 	?	;whether input a is negative(1) or positive(0)
	BUFFERX			dd	?
	BUFFERA1 		dd 	?
	BUFFERA2 		dd 	?
	BUFFERA3 		dd 	?
	FPUSTATUS 		dw 	?
	ERRORMESSAGE  	db	'Error: x < 0 !$'
	CRLF  			db 	0AH,0DH,"$"
	INPUTX			db	'INPUT: x = $'
	INPUTA1			db	'INPUT: a1 = $'
	INPUTA2			db	'INPUT: a2 = $'
	INPUTA3			db	'INPUT: a3 = $'
.CODE

INPUT	proc	near	;read one single character
	mov ah, 01h
	int 21h
	ret
INPUT	endp

OUTPUT	proc	near
	mov ah, 02h
	mov dl, al
	int 21h
	ret
OUTPUT	endp

.STARTUP
	fldz			;push 0.0 into stack
	lea dx, INPUTX
	mov ah, 09h
	int 21h
	call INPUT
	.if al == '-'	;if x is negative, do error handling directly
		jmp ERROR
	.endif
	jmp NEXT

ERROR:				;output error and quit
	lea edx, CRLF
	mov ah, 09h
	int 21h
	lea edx, ERRORMESSAGE
	mov ah, 09h
	int 21h
	mov ah, 4ch
	int 21h

NEXT:
	.REPEAT			;integral part
		fmul TEN
		mov ah, 0
		sub al, 30h
		mov TEMP, ax
		fiadd temp
		call INPUT
	.UNTIL al < '0' || al > '9'
	cmp al, '.'
	jne NEXTA1
	fld1
	.while 1
		fdiv TEN
		call INPUT
		.break	.if al < '0' || al > '9'
		mov ah, 0
		sub al, 30h
		mov TEMP, ax
		fild TEMP
		fmul ST, ST(1)
		fadd ST(2), ST
		fcomp
	.endw
	fcomp
NEXTA1:
	fstp BUFFERX

LOOPA11:
	lea dx, INPUTA1
	mov ah, 09h
	int 21h
	mov SIGN, 0
	fldz
	call INPUT
	.if al == '-'
		mov SIGN, 1
		call INPUT
	.endif
	.REPEAT
		fmul TEN
		mov ah, 0
		sub al, 30h
		mov TEMP, ax
		fiadd TEMP
		call INPUT
	.UNTIL al < '0' || al > '9'
	cmp al, '.'
	jne LOOPA12
	fld1
	.while 1
		fdiv TEN
		call INPUT
		.break	.if al < '0' || al > '9'
		mov ah, 0
		sub al, 30h
		mov TEMP, ax
		fild TEMP
		fmul ST, ST(1)
		fadd ST(2), ST
		fcomp
	.endw
	fcomp
LOOPA12:
	.if SIGN == 1
		fchs
	.endif
	fstp BUFFERA1

LOOPA21:
	lea dx, INPUTA2
	mov ah, 09h
	int 21h
	mov SIGN, 0
	fldz
	call INPUT
	.if al == '-'
		mov SIGN, 1
		call INPUT
	.endif
	.REPEAT
		fmul TEN
		mov ah, 0
		sub al, 30h
		mov TEMP, ax
		fiadd TEMP
		call INPUT
	.UNTIL al < '0' || al > '9'
	cmp al, '.'
	jne LOOPA22
	fld1
	.while 1
		fdiv TEN
		call INPUT
		.break	.if al < '0' || al > '9'
		mov ah, 0
		sub al, 30h
		mov TEMP, ax
		fild TEMP
		fmul ST, ST(1)
		fadd ST(2), ST
		fcomp
	.endw
	fcomp
LOOPA22:
	.if SIGN == 1
		fchs
	.endif
	fstp BUFFERA2

LOOPA31:
	lea dx, INPUTA3
	mov ah, 09h
	int 21h
	mov SIGN, 0
	fldz
	call INPUT
	.if al == '-'
		mov SIGN, 1
		call INPUT
	.endif
	.REPEAT
		fmul TEN
		mov ah, 0
		sub al, 30h
		mov TEMP, ax
		fiadd TEMP
		call INPUT
	.UNTIL al < '0' || al > '9'
	cmp al, '.'
	jne LOOPA32
	fld1
	.while 1
		fdiv TEN
		call INPUT
		.break	.if al < '0' || al > '9'
		mov ah, 0
		sub al, 30h
		mov TEMP, ax
		fild TEMP
		fmul ST, ST(1)
		fadd ST(2), ST
		fcomp
	.endw
	fcomp
LOOPA32:
	.if SIGN == 1
		fchs
	.endif
	fstp BUFFERA3

;calculation
	fstcw FPUSTATUS
	or FPUSTATUS, 0c00h
	fldcw FPUSTATUS

	fld BUFFERX
	fsqrt				;sqrt(x)
	fld BUFFERA1
	fmul
	fst RESULT			;a1*sqrt(x)

	fld BUFFERA2
	fld BUFFERX
	fyl2x				;a2*log2(x)
	fadd RESULT
	fst RESULT

	fld BUFFERX
	fsin
	fld BUFFERA3
	fmul				;a3*sinx
	fadd RESULT
	fstp RESULT

;output
	fstcw FPUSTATUS
	or FPUSTATUS, 0c00h
	fldcw FPUSTATUS

	fld RESULT
	ftst
	fstsw ax

	and ax, 4500h
	cmp ax, 0100h
	jnz POSITIVE
	mov al, '-'
	call OUTPUT
	fabs
POSITIVE:
	fld st
	frndint
	fist INTEGRAL
	fsubr
	fabs
	fstp FRACTIONAL
	mov eax, INTEGRAL
	mov ebx, 10
	mov cx, 0
	push bx
LOOPER1:
	mov edx, 0
	div ebx
	add dl, 30h
	push dx
	inc cx
	cmp eax, 0
	jnz LOOPER1
PRINT:
	pop ax
	call OUTPUT
	loop PRINT
	mov al, '.'
	call OUTPUT
	mov eax, FRACTIONAL

	fstcw FPUSTATUS
	xor FPUSTATUS, 0c00h
	fldcw FPUSTATUS

	fld FRACTIONAL
	fxtract				;extract exponent
	fstp FRACTIONAL
	fabs
	fistp INTEGRAL
	mov ecx, INTEGRAL
	mov eax, FRACTIONAL
	sal eax, 9
	rcr eax, cl
LOOPER2:
	mul ebx
	push eax
	xchg eax, edx
	add al, 30h
	call OUTPUT
	pop eax
	cmp eax, 0
	jnz LOOPER2
QUIT:
	mov ah, 4ch
	int 21h
end