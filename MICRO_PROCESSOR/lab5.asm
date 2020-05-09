DATAS SEGMENT
    BUFFERSIZE  db  128
    BYTESREAD   db  ?
    BUFFER      db  128 dup(?)
    OPRATOR      db  64 dup(?)
    RETURNH     db  ?
    RETURNL     db  ?
    STRING      db  'RESULT:$'
DATAS ENDS

CODES SEGMENT
    ASSUME CS: CODES, DS: DATAS

START:
    mov ax, DATAS
    mov ds, ax
;read string, start from BUFFER
    mov ah, 0ah
    lea dx, BUFFERSIZE
    int 21h
;calculation
    mov bl, BYTESREAD
    mov bh, 00h
    mov di, bx
    lea bx, BUFFER
    mov BYTE PTR [bx + di], 24h     ;'$'
    mov di, 0
    mov si, 0
;push terminal '$' to stack
    lea bx, OPRATOR
    mov BYTE PTR [bx + si], 24h
    mov ax, 0000h
NEXT:
    lea bx, BUFFER
    mov dl, [bx + di]
;if sequence ends
    cmp dl, 24h
    jz EXIT
    cmp dl, 30h
    jnc NUMBER                      ;currently process number
;oprators
SIGNAL:
    cmp dl, 28h                     ;if is '(', check for -
    jnz LABEL1
    lea bx, BUFFER
    mov dl, [bx + di + 1]
    cmp dl, 2dh                     ;input minus number
    jnz LABEL1
    mov ax, 0000h
    push ax                         ;create oprate number 0
LABEL1:
    lea bx, BUFFER
    mov dl, [bx + di]
    cmp dl, 41                      ;check for ')'
    jz RB
    lea bx, OPRATOR
    inc si
    mov BYTE PTR [bx + si], dl
    inc di
    jmp NEXT
RB:
    dec si
    lea bx, OPRATOR
    mov cl, [bx + si]
    cmp cl, 43
    jc  JUMP
    pop ax
    call CALCULATE
    push ax
    mov ax, 0000h
JUMP:
    inc di
    jmp NEXT
;numbers
NUMBER:
    mov cl, 04h
    shl ax, cl
    sub dl, 30h
    add al, dl                      ;form BCD number
    lea bx, BUFFER
    mov dl, [bx + di + 1]
    cmp dl, 30h
    jc NUMBEREND
    inc di
    jmp NEXT
NUMBEREND:
    lea bx, OPRATOR
    mov cl, [bx + si]
    cmp cl, 43
    jnc DOCAL                       ;if is '+' or '-'
NOCAL:
    push ax
    mov ax, 0000h
    inc di
    jmp NEXT
DOCAL:
    call CALCULATE
    push ax
    mov ax, 0000h
    inc di
    jmp NEXT
EXIT:
    lea dx, STRING
    mov ah, 09h
    int 21h
    lea bx, OPRATOR
    mov cl, [bx + si]
    cmp cl, 24h
    jz PRINT
    pop ax
    call CALCULATE
    push ax
PRINT:
    pop bx
    cmp bh, 10h
    jc LABEL3
    mov dl, 45
    mov ah, 02h
    int 21h
LABEL3:
    mov ah, 02h
    mov dl, bh
    and dl, 0fh
    add dl, 30h
    int 21h
    mov dl, bl
    mov cl, 4
    shr dl, cl
    add dl, 30h
    int 21h
    mov dl, bl
    and dl, 0fh
    add dl, 30h
    int 21h
    mov ah, 4ch
    int 21h

CALCULATE   proc    near            ;calculate until no calcalation available, bx contains the first number, and ax the second, result in ax
;store IP address for return    
    pop bx
    mov RETURNH, bh
    mov RETURNL, bl
;pop oprate number out, operand in cl
LABEL2:
    pop bx
    cmp bh, 10h
    jnc FN
FP:                                 ;first operand is positive
    cmp ah, 10h
    jnc FPSN
;first oprand positive and second oprand positive
FPSP:                               
    cmp cl, 43                      ;if it's addition
    jz ADD1
SUB1:                                ;bx - ax
    cmp bx, ax
    jnc NOCHANGE
CHANGE:
    mov cx, ax
    mov ax, bx
    mov bx, cx
    call SUBTRACTION
    or ah, 10h
    jmp AFTERCAL
NOCHANGE:
    call SUBTRACTION
    and ah, 0fh
    jmp AFTERCAL
ADD1:
    call ADDITION
    jmp AFTERCAL
;first oprand positive and second oprand negative
FPSN:
    cmp cl, 43
    jz ADD2
SUB2:                               ;bx - (-ax)
    and ah, 0fh
    call ADDITION
    jmp AFTERCAL
ADD2:                               ;bx - ax
    and ah, 0fh
    cmp bx, ax
    jnc NOCHANGE2
CHANGE2:
    mov cx, ax
    mov ax, bx
    mov bx, cx
    call SUBTRACTION
    or ah, 10h
    jmp AFTERCAL
NOCHANGE2:
    call SUBTRACTION
    and ah, 0fh
    jmp AFTERCAL
CARRYON:
    jmp LABEL2
FN:
    cmp ah, 10h
    jnc FNSN
FNSP:
    cmp cl, 2bh
    jz ADD3
SUB3:                               ;(-bx) - ax
    and bh, 0fh
    call ADDITION
    or ah, 10h
    jmp AFTERCAL
ADD3:                               ;(-bx) + ax
    and bh, 0fh                     ; do ax - bx
    cmp bx, ax
    jc NOCHANGE3
CHANGE3:                            ;bx >= ax
    call SUBTRACTION
    or ah, 10h
    jmp AFTERCAL
NOCHANGE3:                          ;bx < ax
    mov cx, ax
    mov ax, bx
    mov bx, cx
    call SUBTRACTION
    and ah, 0fh
    jmp AFTERCAL
FNSN:
    cmp cl, 2bh
    jz ADD4
SUB4:                               ;(-bx)-(-ax)
    and bh, 0fh
    and ah, 0fh                     ;ax - bx
    jmp ADD3
ADD4:                               ;(-bx) + (-ax)
    and bh, 0fh
    and ah, 0fh
    call ADDITION
    or ah, 10h
    jmp AFTERCAL
AFTERCAL:
    dec si
    mov bx, OFFSET OPRATOR
    mov cl, [bx + si]
    cmp cl, 43
    jnc CARRYON
    mov bh, RETURNH
    mov bl, RETURNL
    push bx
    ret
CALCULATE endp

ADDITION    proc    near            ;calculate bx + ax, result in ax, bx and ax positive, result is positive
    add ah, bh
    add al, bl
    daa
    adc ah, 0
    mov bl, al
    mov al, ah
    daa
    mov ah, al
    mov al, bl
    and ah, 0fh
    ret
ADDITION    endp

SUBTRACTION proc    near            ;calculate bx - ax, result in ax, bx and ax positive, result is positive
    sub bl, al
    mov al, bl
    das
    sbb bh, 0
    sub bh, ah
    mov ah, bh
    and ah, 0fh
    ret
SUBTRACTION  endp

CODES ENDS
END START