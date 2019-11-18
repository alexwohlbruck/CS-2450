		.orig x3000
		JSR GETH4
		JSR GETH4
		HALT

GETHD	ST R7, SAVER7
		ST R5, SAVER5
		ST R4, SAVER4
		ST R3, SAVER3
GHD_TOP	GETC
		; if (R0 >= 48 && R0 <= 57)
		NOT R3, R0 ; Multiply R0 by -1 and store in R3
		ADD R3, R3, #1
		LD R5, ASCIINL
		ADD R4, R3, R5
		BRp ENDIF1 ; R0 < 48, test failed
		NOT R3, R0
		ADD R3, R3, #1
		LD R5, ASCIINU
		ADD R4, R3, R5
		BRn ENDIF1 ; R0 > 57, test failed
		OUT ; Test passed, print & keep decimal value in R0
        LD R5, HEXNTD
        ADD R0, R0, R5 ; Convert to decimal value
        BRnzp GHD_END
ENDIF1	; Test failed, continue
		; if (R0 >= 65 && R0 <= 70)
		NOT R3, R0 ; Multiply R0 by -1 and store in R3
		ADD R3, R3, #1
		LD R5, ASCIIUL
		ADD R4, R3, R5
		BRp ENDIF2 ; R0 < 48, test failed
		NOT R3, R0
		ADD R3, R3, #1
		LD R5, ASCIIUU
		ADD R4, R3, R5
		BRn ENDIF2 ; R0 > 57, test failed
		OUT ; Test passed, print
        LD R5, HEXUTD
        ADD R0, R0, R5 ; Convert to decimal value
        BRnzp GHD_END
ENDIF2	; Test failed, continue
		; if (R0 >= 97 && R0 <= 102)
		NOT R3, R0 ; Multiply R0 by -1 and store in R3
		ADD R3, R3, #1
		LD R5, ASCIILL
		ADD R4, R3, R5
		BRp ENDIF3 ; R0 < 48, test failed
		NOT R3, R0
		ADD R3, R3, #1
		LD R5, ASCIILU
		ADD R4, R3, R5
		BRn ENDIF3 ; R0 > 57, test failed
		OUT ; Test passed, print
        LD R5, HEXLTD
        ADD R0, R0, R5 ; Convert to decimal value
        BRnzp GHD_END
ENDIF3	; Test failed, restart
		BRnzp GHD_TOP
GHD_END
		LD R5, SAVER5
		LD R4, SAVER4
		LD R3, SAVER3
		LD R7, SAVER7
		RET
		
MULT    ST R3, SAVER3
        AND R0, R0, #0
        ADD R1, R1, #1
        ADD R1, R1, #-1
        BRZ FINAL
        ADD R2, R2, #1
        ADD R2, R2, #-1
        BRZ FINAL
        AND R0, R0, #0
        NOT R3, R1
        ADD R3, R3, #1
        ADD R3, R3, R2
        BRN MULT2
        AND R3, R3, #0
        ADD R3, R1, #0
MULT1   ADD R0, R0, R2
        ADD R3, R3, #-1
        BRP MULT1
        BRNZP FINAL
MULT2   AND R3, R3, #0
        ADD R3, R2, #0
MULT3   ADD R0, R0, R1
        ADD R3, R3, #-1
        BRP MULT3
FINAL   LD R3, SAVER3
        RET

GETH4	ST R7, SAVER72
		ST R1, SAVER1
		ST R2, SAVER2
		ST R6, SAVER6
		
		AND R6, R6, #0
		
		JSR GETHD
		ST R0, SAVER0
		LD R1, SAVER0
		LD R2, HEX3
		JSR MULT
		ADD R6, R6, R0

		JSR GETHD
		ST R0, SAVER0
		LD R1, SAVER0
		LD R2, HEX2
		JSR MULT
		ADD R6, R6, R0
		
		JSR GETHD
		ST R0, SAVER0
		LD R1, SAVER0
		LD R2, HEX1
		JSR MULT
		ADD R6, R6, R0
		
		JSR GETHD
		ADD R6, R6, R0

		; Print new line
		LD R0, ASCIICR
		OUT

		ST R6, SUM
		LD R0, SUM
		
		LD R7, SAVER72
		LD R1, SAVER1
		LD R2, SAVER2
		LD R6, SAVER6
		RET

SAVER7 	.fill	#0
SAVER72	.fill	#0
SAVER6	.fill	#0
SAVER5	.fill	#0
SAVER4	.fill	#0
SAVER3	.fill	#0
SAVER2	.fill	#0
SAVER1	.fill	#0
SAVER0	.fill	#0
ASCIICR	.fill	#10 ; ASCII new line return
ASCIINL .fill	#48 ; Lower limit of ASCII numbers 0
ASCIINU .fill	#57 ; Upper limit of ASCII numbers 9
ASCIIUL	.fill	#65 ; Lower limit of ASCII uppercase A
ASCIIUU	.fill	#70 ; Upper limit of ASCII uppercase F
ASCIILL	.fill	#96 ; Lower limit of ASCII lowercase a
ASCIILU	.fill	#102 ; Upper limit of ASCII lowercase f
HEXNTD  .fill   #-48 ; Convert hexadecimal number ASCII code to decimal value
HEXUTD  .fill   #-55 ; Convert hexadecimal uppercase ASCII code to decimal value
HEXLTD  .fill   #-87 ; Convert hexadecimal lowercase ASCII code to decimal value
SUM		.fill 	#0
HEX1	.fill	#16
HEX2	.fill	#256
HEX3	.fill	#4096
.end