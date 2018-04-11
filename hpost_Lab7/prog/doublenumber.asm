; CS 350, Fall 2016.
; Henry Post, hpost@hawk.iit.edu
;
;
;
;
;
;
;
;
;
;

       .ORIG 	x3100		; begin @ x3100
	LD	R1, SIX
	LD	R2, NUMBER
	AND	R3, R3, #0	; clear R3	

	
AGAIN	ADD R3, R3, R2
	ADD R1, R1, #-1
	BRp	AGAIN
	
	HALT	
	
NUMBER	.FILL 6
SIX	.FILL 6




	HALT


prompt	.STRINGZ "Enter a number to be doubled: "
retChar	.STRINGZ "\n"
	.BLKW 	80		; buffer space for strings
	.END