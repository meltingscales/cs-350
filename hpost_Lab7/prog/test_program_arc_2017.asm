;
;
;
;   123 -> ['1', '2', '3']...how?
;   
;
;   123 / 100 = 1
;   23  / 10  = 2
;   3   / 1   = 3
;
;
;
; IMM5: b  b  b  b  b   <- 2's complement
;       +- n  n  n  n
;       -(1111) -> +(1111) 
;       -16 -> + 16
;  +100 -> 1100100
;
;
;
;




.ORIG x1234													; makes the beginning of memory be x1234



					LD	R0, varX							; R0 = mem[varX]


LOOP1:		ADD	R0, R0, #0						; set cc to the sign of R0
					BRN		END									; if r0 < 0: goto END
					ADD		R0, R0, #-3					; r0 = r0 - 3 
					ADD		R1, R1, #1					; r1++
					BR		LOOP1								; goto LOOP1


END:			LEA	R0, message						; R0 = &message
					PUTS											; printf(R0)


message		.STRINGZ 	"end of loop!"		; mem[message] gets filled with a sequence of chars
varX			.FILL			100								; mem[varX] = 100



.END
