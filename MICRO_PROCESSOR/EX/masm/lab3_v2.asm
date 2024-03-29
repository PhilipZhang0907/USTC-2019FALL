DATAS SEGMENT
    STRING DB 'INPUT3.txt',0
    DATA DB 512 DUP(?)
    OU DB 512 DUP(?)
    FUB DW 128 DUP(?)
    LEN DW 000AH
    BEF DW 0000H
    AFT DW 0000H
    OUTNOW DW 0000H;now 
    THOU DW 03E8H
    HAS DW 0064H
    TEN DW 000AH
    TC DW 0000H
    SETC DW 0000H
    SETCOUNT DB 00H
    TEMP DW 0000H
    HAND DW ?
    NUM DW ?
DATAS ENDS
CODES SEGMENT
    ASSUME CS:CODES，DS:DATAS
START:
    MOV AX,DATAS
    MOV DS,AX

    LEA DX,STRING
    MOV AL,00H
    MOV AH,3DH
    INT 21H

    MOV HAND,AX
    MOV BX,AX
    MOV CX,00FFH
    MOV AH,3FH
    LEA DX,DATA
    INT 21H

    MOV NUM,AX
    MOV BX,HAND
    MOV AH,3EH
    INT 21H

L1: MOV AX,0000H
    LEA BX,DATA
    ADD BX,BEF
    MOV CL,BYTE PTR [BX]
    CMP CL,'-'
    JNE L3;>0
    INC BEF
    JMP L2;

L2: LEA BX,DATA
    ADD BX,BEF
    MOV CL,BYTE PTR [BX]
    CMP CL,' '
    JNE LN1
    ADD AX,8000H
    LEA BX,FUB
    ADD BX,AFT
    MOV [BX],AX
    ADD AFT,0002H
    INC TC
    INC BEF
    JMP L1
LN1: MUL LEN
    LEA BX,DATA
    ADD BX,BEF
    SUB BYTE PTR [BX],30H
    ADD AL,[BX]
    INC BEF
    MOV BX,NUM
    CMP BEF,BX
    JB L2
    ADD AX,8000H
    LEA BX,FUB
    ADD BX,AFT
    MOV [BX],AX
    ADD AFT,0002H
    INC TC
    JMP L4

L3: LEA BX,DATA
    ADD BX,BEF
    MOV CL,BYTE PTR [BX]
    CMP CL,' '
    JNE LN2
    LEA BX,FUB
    ADD BX,AFT
    MOV [BX],AX
    ADD AFT,0002H
    INC TC
    INC BEF
    JMP L1
LN2: MUL LEN
    LEA BX,DATA
    ADD BX,BEF
    SUB BYTE PTR [BX],30H
    ADD,AL,[BX]
    INC BEF
    MOV BX,NUM
    CMP BEF,BX
    JB L3
    LEA BX,FUB
    ADD BX,AFT
    MOV [BX],AX
    ADD AFT,0002H
    INC TC
    JMP L4

L4: MOV CX,TC
    DEC CX
SETB: MOV SETCOUNT,00H
    MOV SETC,0000H
SE1: LEA BX,FUB
    ADD BX,SETC
    MOV AX,WORD PTR [BX]
    ADD BX,0002H
    CMP AX,WORD PTR [BX]
    JG SE2
    LEA BX,FUB
    ADD BX,SETC
    MOV AX,WORD PTR [BX]
    CMP AX,0000H
    JL SS1
    JMP G1
SS1: LEA BX,FUB
    ADD BX,SETC
    ADD BX,0002H
    MOV AX,WORD PTR [BX]
    CMP AX,0000H
    JL G2
    JMP G1
    
G1: LEA BX,FUB
    ADD BX,SETC
    MOV AX,WORD PTR [BX]
    MOV TEMP,AX
    ADD BX,0002H
    MOV AX,WORD PTR [BX]
    SUB BX,0002H
    MOV WORD PTR [BX],AX
    MOV AX,TEMP
    ADD BX,0002H
    MOV WORD PTR [BX],AX
    JMP G2

SE2: LEA BX,FUB
    ADD BX,SETC
    MOV AX,WORD PTR [BX]
    CMP AX,0000H
    JL SE3
    JMP G2
SE3:  LEA BX,FUB
    ADD BX,SETC
    ADD BX,0002H
    MOV AX,WORD PTR [BX]
    CMP AX,0000H
    JL G1
    JMP G2

G2: INC SETCOUNT
    ADD SETC,0002H
    CMP SETCOUNT,CL
    JB QQ1
    JMP QQ2
QQ1: JMP SE1
QQ2: DEC CX
    CMP CX,0000H
    JNE QQ3
    JMP L5
QQ3: JMP SETB

L5: MOV CX,TC
    MOV TC,0000H
    MOV AX,AFT
    MOV OUTNOW,AX
    SUB OUTNOW,0002H

BEGIN:  LEA BX,FUB
    ADD BX,OUTNOW
    CMP WORD PTR [BX],0000H
    JL L6
    JMP L7

L6: LEA BX,OU
    ADD BX,TC
    MOV BYTE PTR [BX],'-'
    INC TC
    LEA BX,FUB
    ADD BX,OUTNOW
    SUB WORD PTR [BX],8000H

L7: LEA BX,FUB
    ADD BX,OUTNOW
    MOV AX,WORD PTR [BX]
    CMP AX,THOU
    JB Q1
    SUB AX,THOU
    LEA BX,OU
    ADD BX,TC
    MOV BYTE PTR [BX],'1'
    INC TC

Q1: CMP AX,HAS
    JB Q2
    MOV DX,0000H
    DIV HAS
    ADD AL,30H
    LEA BX,OU
    ADD BX,TC
    MOV BYTE PTR [BX],AL
    INC TC
    MOV AX,DX
    JMP L8
Q2: LEA BX,OU
    ADD BX,TC
    MOV BYTE PTR [BX],'0'
    INC TC

L8: CMP AX,TEN
    JB Q3
    MOV DX,0000H
    DIV TEN
    ADD AL,30H
    LEA BX,OU
    ADD BX,TC
    MOV BYTE PTR [BX],AL
    INC TC
    MOV AX,DX
    JMP L9
Q3: LEA BX,OU
    ADD BX,TC
    MOV BYTE PTR [BX],'0'
    INC TC
L9: ADD AL,30H
    LEA BX,OU
    ADD BX,TC
    MOV BYTE PTR [BX],AL
    INC TC
    LEA BX,OU
    ADD BX,TC
    MOV BYTE PTR [BX],' '
    INC TC
    SUB OUTNOW,0002H
    DEC CX
    JZ NZ1
    JMP BEGIN

NZ1: LEA BX,OU
    ADD BX,TC
    MOV BYTE PTR [BX],'$'
    INC TC

    LEA DX,OU
    MOV AH,09H
    INT 21H

    MOV AX,4C00H
    INT 21H
CODES ENDS
END START
