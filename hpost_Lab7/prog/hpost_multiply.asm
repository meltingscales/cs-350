; CS 350, Fall 2016.
; Henry Post, hpost@hawk.iit.edu
;
;
; Multiplies X by Y and stores it in Z.
;
;
;
;
;
;
;


       .ORIG 	x3100		; begin @ x3100


	LD	R2, X		; R2 = x
	AND	R3, R3, 0	; R3 = x*(y-k)
	LD	R1, Y

mloop	BRNZ	done		; loop until k <= 0
	ADD	R3, R3, R2	; R3 = R3 + x
	ADD	R1, R1, -1	; k--
	
	BR	mloop

done	ST	R3, product	; product = x*y
	HALT

X	.FILL	12
Y	.FILL	8
product	.BLKW	1		; stores x*y
	.END