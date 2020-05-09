DATAS SEGMENT
    STRINGA  DB  'Input1.txt',0
    STRINGB  DB  'Output1.txt',0
    BUFFER   DB  80H
             DB  ?
             DB  80H  DUP(0)
    BUFFERB  DB  80H
             DB  ?
    BUFFERD  DB  80H  DUP(0)
DATAS ENDS

CODES SEGMENT
    ASSUME CS:CODES, DS:DATAS
    
START:
    MOV AX, DATAS
    MOV DS, AX
;Create File Input1.txt
    LEA DX, STRINGA
    MOV CX, 00H
    MOV AH, 3CH
    INT 21H
;Ready for keyboard input
    LEA DX, BUFFER
    MOV AH, 0AH
    INT 21H
;Open File Input1.txt write
    LEA DX, STRINGA
    MOV AL, 3
    MOV AH, 3DH
    INT 21H
;Write file
    MOV CX, 80H
    MOV BX, AX
    LEA DX, BUFFER+2
    MOV AH, 40H
    INT 21H
;Close file
    MOV AH, 3EH
    INT 21H

;FIRST PART FINISH
    
;Open File Input1.txt read
    MOV DX, OFFSET STRINGA
    MOV AL, 2H
    MOV AH, 3DH
    INT 21H
;Read File
    MOV BX, AX
    MOV DX, OFFSET BUFFERD
    MOV CX, 80H
    MOV AH, 3FH
    INT 21H
    MOV CX, AX
    MOV AH, 3EH
    INT 21H
;Ajust
    MOV BX, CX
    MOV SI, OFFSET BUFFERD
NEXT:
    MOV DL, [SI]
    CMP DL, 61H
    JB  DONOTSUB
    SUB DL, 20H
    MOV [SI], DL
DONOTSUB:
    INC SI
    MOV AH, 02H
    INT 21H
    LOOP NEXT

    LEA DX, STRINGB
    MOV CX, 00H
    MOV AH, 3CH
    INT 21H
;Open File Output1.txt write
    MOV CX, BX
    MOV BX, AX
    MOV DX, OFFSET BUFFERD
    MOV AH, 40H
    INT 21H
;Close file
    MOV AH, 3EH
    INT 21H
;exit
    MOV AX, 4C00H
    INT 21H
CODES ENDS
END START
