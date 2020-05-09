;number sequence like -23/46/1024/$
DATAS SEGMENT
    STRING     DB 'INPUT3.txt',0
    READ       DB 900 DUP(0)        ;Store the ASCII
    BUFFER     DB 222 DUP(0)        ;Store the numbers
DATAS ENDS

CODES SEGMENT
    ASSUME CS:CODES, DS:DATAS

START:
    mov ax, DATAS
    mov ds, ax
;open file
    mov dx, OFFSET STRING
    mov al, 02h
    mov ah, 3dh
    int 21h
;read file
    mov dx, OFFSET READ
    mov bx, ax
    mov cx, 0384H
    mov ah, 3fh
    int 21h
    mov ah, 3eh
    int 21h
;
    lea di, BUFFER
    lea si, READ
NEXT:
    call ADJUST
    mov BYTE PTR [di], ah
    mov BYTE PTR [di + 1], al
    inc si
    mov bl, [si]
    cmp bl, 24h         ;'$'
    jz COMPARE
    add di, 02h
    jmp NEXT

COMPARE:
    add di, 02h
    mov BYTE PTR [di], 240
    mov BYTE PTR [di + 1], 240
    mov si, OFFSET BUFFER
LABEL3:
    mov di, si
    add di, 02h
    mov bl, [di]
    cmp bl, 240
    jz RETURN
LABEL2:
    call COMPARISON     ;compare si and di, if si > di, change directly
    add di, 02h
    mov bl, [di]
    cmp bl, 240
    jnz LABEL2
    call PRINT          ;print number si points, and si point to next number
    jmp LABEL3
RETURN:
    call PRINT
    mov ah, 4ch
    int 21h

ADJUST  proc    near    ;SI ends at '/' and AX contains BCD number
    mov ax, 0000h
    mov cl, 04h
    mov bl, [si]
    cmp bl, 2dh     ;'-'
    jz NEGATIVE
POSITIVE:
    shl ax, cl
    sub bl, 30h
    add al, bl
    inc si
    mov bl, [si]
    cmp bl, 2fh     ;'/'
    jz  EXITP
    jmp POSITIVE
NEGATIVE:
    inc si
    mov bl, [si]
    cmp bl, 2fh     ;'/'
    jz EXIT
    shl ax, cl
    sub bl, 30h
    add al, bl
    jmp NEGATIVE
EXIT:
    add ah, 160     ;ah > A0h means it's negative
EXITP:
    ret
ADJUST  endp    


COMPARISON  proc    near
    mov ah, [si]
    mov al, [si + 1]
    mov bh, [di]
    mov bl, [di + 1]
    cmp ah, 160
    jnc FMINUS
    jmp FPOSITIVE
FMINUS:
    cmp bh, 160
    jc  EXIT2
NN:
    cmp ax, bx
    jnc  EXIT2
    push ax
    mov ax, bx
    pop bx
    mov BYTE PTR [si], ah
    mov BYTE PTR [si + 1], al
    mov BYTE PTR [di], bh
    mov BYTE PTR [di + 1], bl
    jmp EXIT2
FPOSITIVE:
    cmp bh, 160
    jc  PP
PN:
    push ax
    mov ax, bx
    pop bx
    mov BYTE PTR [si], ah
    mov BYTE PTR [si + 1], al
    mov BYTE PTR [di], bh
    mov BYTE PTR [di + 1], bl
    jmp EXIT2
PP:
    cmp ax, bx
    jc  EXIT2
    jmp PN
EXIT2:
    ret
COMPARISON endp

PRINT   proc    near
    mov bh, [si]
    mov bl, [si + 1]
    cmp bh, 160
    jc  P
N:
    mov dl, 2dh
    sub bh, 160
    mov ah, 02h
    int 21h
P:
    mov ah, 02h
    mov dl, bh
    mov cl, 4
    shr dl, cl
    add dl, 30h
    int 21h
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
    add si, 02h
    mov dl, 2eh
    int 21h
    ret
PRINT endp

CODES ENDS
END START
