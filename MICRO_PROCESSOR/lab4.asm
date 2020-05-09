DATAS SEGMENT
    BUFFERSIZE   db   2 dup(0AH)
    BUFFER db   2 dup(0AH)
    ARG    DB   1   DUP(?)
    CARRY  DB   1   DUP(0)
    ARGU   DB   1   DUP(1)
    RESULT db   1   DUP(1)
    UNKNOW db   10  dup(0AH)
DATAS ENDS

CODES SEGMENT
    ASSUME CS: CODES, DS: DATAS

START:
    MOV AX, DATAS
    MOV DS, AX
;read input
    LEA DX, BUFFERSIZE
    MOV AH, 0AH
    INT 21H
    MOV AL, [DI+1]
    LEA DI, BUFFER
    MOV AL, [DI]
    SUB AL, 30H
    INC DI
    MOV BL, [DI]
    cmp BL, 0DH
    jz  CALC
    MOV CL, 4
    SHL AL, CL
    MOV BL, [DI]
    SUB BL, 30H
    ADD AL, BL
CALC:
    MOV ARG, AL
    MOV ARGU, 01H
    MOV CH, 0
    MOV CL, ARG
    MOV AX, CX
    INC AX
    daa
    MOV CX, AX
LP3:
    push CX
    MOV CARRY, 00H
    call COMPUTE
    INC ARGU
    MOV AL, ARGU
    daa
    MOV ARGU, AL
    pop CX
    cmp AL, CL
    jnz LP3
    LEA DI, RESULT
    MOV CX, 0
PRESS:
    MOV BL, [DI]
    MOV BH, 0
    push BX
    INC CX
    INC DI
    MOV AL, [DI]
    cmp AL, 0AH
    jnz PRESS
PRINT:
    MOV AH, 02H
    pop DX
    push DX
    MOV BX, CX
    MOV CL, 4
    SHR DL, CL
    AND DL, 0FH
    ADD DL, 30H
    INT 21H
    pop DX
    AND DL, 0FH
    ADD DL, 30H
    INT 21H
    MOV CX, BX
    LOOP PRINT
;
    MOV AH,4CH
    INT 21H 

COMPUTE PROC NEAR   ;multiple result value with argu
    LEA DI, ARGU
    MOV BH, [DI]
    MOV BL, 0
    LEA DI, RESULT
NEXT:
    push BX
    MOV BL, [DI]
    call FUNC
    MOV AL, CARRY
    MOV AH, 0
    ADD AX, DX      ;add result DX with carry
    daa 
    ADC AH, 0      ;if a carry flag after correction
    MOV BYTE PTR [DI], AL
    MOV CARRY, AH   ;load present result
    INC DI
    MOV BL, [DI]
    cmp BL, 0AH
    jz EXIT
    pop BX
    jmp NEXT
EXIT:
    pop BX
    MOV AL, CARRY
    cmp AL, 0
    jz  JUMP
    MOV BYTE PTR [DI], AL
JUMP:
    RET
COMPUTE ENDP

FUNC    PROC NEAR   ;compute BH x BL in BCD codes, result in DX
    MOV DX, 0
    MOV CL, BH
    AND CL, 0FH
    MOV CH, 0
    cmp CX, 0000H
    jz LAL
LP1:
    MOV AL, DL
    ADD AL, BL
    daa
    MOV DL, AL

    MOV AL, DH
    ADC AL, 0
    daa
    MOV DH, AL
    LOOP LP1
LAL:
    MOV CL, 4
    SHR BH, CL
    MOV CL, BH
    MOV CH, 0

    push CX
    MOV CL, 4
    SHL BX, CL
    AND BX, 0FF0H
    pop CX
    cmp CX, 0000H
    jz  RETT
LP2:
    MOV AL, DL
    ADD AL, BL
    daa
    MOV DL, AL

    MOV AL, DH
    ADC AL, BH
    daa
    MOV DH, AL
    LOOP LP2
;
RETT:
    RET
FUNC    ENDP

CODES ENDS
END START