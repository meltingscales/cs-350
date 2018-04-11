; 
; 
; Henry Post, hpost@hawk.iit.edu
; 
; 
; 
; This reads a char to r0, store, read, store, until newline.
; i.e. reads a "*\n" into a buffer thing. yeah.
; 
; 
; 
; ourStr[7] = {0}
; strIndex = 0
; done = false
;
; while not done:
;   r0 <- keyBchar  
;   if r0 = '\n':
;     done = true
;   ourStr + strIndex = r0
;   strIndex++
;
;
;
;
;
;



	.ORIG xA455 ; :)

	ADD		R0, R0, 12
	ADD 	R1, R1, 13
	JSR 	multR01

	; if our MULT works, it should make R3 = 12 * 13


	; R0 = temp char and/or pointer to some strings
	; R1 = ~R0 for ugly math
	; R2 = *sBuffer
	; R3 = *nBuffer
	
	; R6 = "\n"

	LEA	R0, msg		; R0 = &msg
	PUTS			; prints *R0





;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; READ LOOP, SAVES TO sBuffer, STOP ON "\n"!
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


read	LD	R6, retChar	; R6 = "\n"
		LEA	R2, sBuffer	; R2 = &buffer
	
rdLoop	GETC			; R0 = input
		OUT			

		NOT	R1, R0		; R1 = -R0-1
		ADD	R1, R1, 1	; R1++
		ADD	R1, R6, R1	; R1 = R1+R6

		BRZ	convCh		; x == "\n", otherwise store char.


		STR	R0, R2, #0	; mem[R2] = R0
		ADD	R2, R2, 1	; R2++
		BR 	rdLoop		; read char





;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; CONVERT CHAR[] TO INT[], sBuffer -> nBuffer
;
; SAVES NUMLEN IN BASE10 TO ourNumL... i.e.
; 
; "23456" -> ourNumL = 5...
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

convCh	LD	R1, zerChar	; R1 = &zerChar aka R1 = "0"

	NOT	R1, R1		; R1 = -R1 - 1
	ADD	R1, R1, 1	; R1++


	; now we have negative "0"... useful!
	

	LEA	R2, sBuffer	; R2 = *sBuffer
	LEA	R3, nBuffer	; R3 = *nBuffer
	
ccLoop	LDR	R0, R2, #0	; R0 = mem[R2]
	
	BRZ	str2Num		; if we have NULL, we're done 
	
	ADD	R0, R0, R1	; R0 = R0 + (-"0")
	STR	R0, R3, #0	; mem[R3] = R0

	ADD	R3, R3, 1	; R3++
	ADD	R2, R2, 1	; R2++

	BR	ccLoop		; repeat 



;;;;;;;;;;;;;
;
; Converts a string array ["1","2","3"] to a number, 123!
;
; sBuffer =  char[], or ["1","2","3"]
;
;;;;;;;;;;;;;



str2Num	NOP

	AND	R0, R0, 0	; clear R0
	AND	R1, R1, 0	; clear R1

	ADD	R1, R1, #10	; R1 = 10


	
	
	LEA	R3, sBuffer	; R3 = *string
	LD	R6, NASCII	; R6 = -"0"

convLp	LDR	R2, R3, #0	; R2 = mem[sBuffer]
	BRZ	endCvLp		; end IF "\0"

	ADD	R2, R2, R6	; R2 = number(R2)

	JSR	multR01		; R2 = R0 * R1, kill R0

	ADD	R0, R2, #0	; R0 = R2. keep multiplying
	
	ADD	R3, R3, #1	; *string++
	BR	convLp
	


endCvLp	ADD	R0, R0, R0	; R0 = R0 * 2

	; ok bro. R0 has double ur nubmer.  NOW MAKE IT A SKI.

	LEA	R4, sBuffer2Z	; null-term for sBuffer*2
	LD	R5, ASCII	; R5 = "0"
cvLp2	ADD	R4, R4, #-1	; step down
	
	JSR	divR01		; R0 = R0/R1, R2 = R0%R1

	ADD	R2, R2, R5	; 4023%1000, aka 3, = 3+"0" = "3"

	STR	R2, R4, #0	; mem[R4] = "3"

	ADD	R0, R0, #0	; to BRZ
	BRP	cvLp2
	
	ST	R4, startSB2	; to avoid printing \0\0\0\0\0 8046

	BR	weGood




;;;;;;;;;;;;;
;
; R0 * R1 gets added to R2.
;
; !!! ~~~ KILLS R0 ~~~ !!!
;
;;;;;;;;;;;;;



multR01

	ADD	R0, R0, #0
	BRNZ	mult0

	ADD	R1, R1, #0
	BRNZ	mult0


mult	ADD	R2, R2, R1	; R2 = R2 + R1
		ADD	R0, R0, #-1	; R0--

		BRP 	mult		; if R1 > 0, keep adding.
		BR 		multR		; return

mult0	ADD	R2, R2, #0	; R2 = R2...nothing to multiply...
		BR 	multR


multR	RET



;;;;;;;;;;;;;;;;;;
;
; R0 / R1 gets put into R0,
; R0 % R1 gets put into R2.
;
;
; This will kill R0, R2, R3.
;
;;;;;;;;;;;;;;;;;;

divR01	AND	R3, R3, #0	; R3 = 0

		ADD	R2, R0, #0	; R2 = R0
	
		NOT	R1, R1		; 
		ADD	R1, R1, #1	; R1 = -R1


divLp	ADD	R2, R2, R1	; R2 = R2 - R1
		BRN	divEnd		; one too far, done dividing

		ADD	R3, R3, #1	; R3++

	BR	divLp		;oops
	
divEnd	NOT	R1, R1
	ADD	R1, R1, #1	; -R1 = R1
	
	ADD	R2, R2, R1	; to undo our one too far. this is remainder.
	
	ADD	R0, R3, #0	; R0 = R3

	RET



;;;;;;;;;;;;;;;;;;;;;
;
; PRINT STATEMENTS BELOW
;
;;;;;;;;;;;;;;;;;;;;;



weGood	LEA	R0, retChar	; R0 = "\n"
	PUTS			; print(R0)
	PUTS			; print(R0)

	LEA	R0, ujt		; R0 = *ujt
	PUTS			; print(R0)

	LEA	R0, sBuffer	; R0 = *sBuffer
	PUTS			; print(R0)

	LEA	R0, nwh		; R0 = *nwh
	PUTS			; print(R0)

	LD	R0, startSB2	; R0 = start of our times 2 string
	PUTS			; print(R0)	

	HALT			; bye




HALT


Stack	.BLKW	#10		; runtime stack
Counter	.FILL	#3
ASCII	.FILL	#48
NASCII	.FILL	#-48


sBuffer	.BLKW	10		; "1234" =  [49, 50, 51, 52]
sBuffer2 .BLKW	10
sBuffer2Z .FILL	0		;null-termination for second string
startSB2  .FILL	42069		;hi

nBuffer	.BLKW	10		; 1234  =  [1, 2, 3, 4] 
nBuffer2 .BLKW	10		

ten	.FILL	xA		; ten = 10

ourNumL	.BLKW	1		; number buffer length
ourNum	.BLKW	1		; actual number

retChar	.STRINGZ "\n"
zerChar	.STRINGZ "0"

test	.STRINGZ "\nTESTIN BRUH\n"

msg	.STRINGZ "Please enter a number and then an...enter: "
ujt	.STRINGZ "You just typed: "
nwh	.STRINGZ "\nOur converted number is: "
test2	.STRINGZ "See R4 for our decimal number"

.END
