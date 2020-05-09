.8087
DATAS SEGMENT
    stringx db  'Input X: $'
    string1 db  'Input a1: $'
    string2 db  'Input a2: $'
    string3 db  'Input a3: $'
    error   db  'Error: x < 0 !$'
    a1      dw  ?
    a2      dw  ?
    a3      dw  ?
    x       dw  ?
    result  dd  ?
    buffersize  db  ?
    readsize    db  ?
    buffer  db  5   dup(?)
DATAS ENDS

CODES SEGMENT
    ASSUME  CS: CODES, DS: DATAS
START:
    mov ax, DATAS
    mov ds, ax
;input x
    lea dx, stringx
    mov ah, 09h
    int 21h
    lea dx, buffersize
    mov ah, 0ah
    int 21h
;check for negative input
    lea bx, buffer
    mov di, 0
    mov cl, [bx + di]
    cmp cl, '-'
    jnz NEXT
ERRORPRINT:
    lea dx, error
    mov ah, 09h
    int 21h
    mov ah, 4ch
    int 21h
NEXT:
    call ADJUST
    mov x, ax
;input a1
    lea dx, string1
    mov ah, 09h
    int 21h
    lea dx, buffersize
    mov ah, 0ah
    int 21h
    call ADJUST
    mov a1, ax
;input a2
    lea dx, string2
    mov ah, 09h
    int 21h
    lea dx, buffersize
    mov ah, 0ah
    int 21h
    call ADJUST
    mov a2, ax
;input a3
    lea dx, string3
    mov ah, 09h
    int 21h
    lea dx, buffersize
    mov ah, 0ah
    int 21h
    call ADJUST
    mov a3, ax
;do calculation
    finit
    fild x
    fsqrt               ;sqrt(x)
    fimul a1            ;a1*sqrt(x)
    fst result
    fild x
    fild a2
    fyl2x               ;a2*log2(x)
    fdecstp
    fild x
    fsin                ;sin(x)
    fimul a3            ;a3*sin(x)
    fadd                ;a2*log2(x) + a3*sin(x)
    fadd result
    fst result
    jmp PRINT
PRINT:
    
EXIT:
    mov ah, 4ch
    int 21h

ADJUST  proc    near
    mov ax, 0000h
ADJUST  endp

CODES ENDS
END START
