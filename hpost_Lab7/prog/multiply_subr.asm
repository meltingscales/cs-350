; CS 350, Fall 2016
;
; multiply_subr.asm -- Multiplication as a subroutine
;
; Illinois Institute of Technology, (c) 2016, James Sasaki

        .ORIG    x3050

; Main program: Calculate X * Y, store result in product
;
         LD      R1, X
         LD      R2, Y
         JSR     Mult
         ST      R1, Z
         HALT

X       .FILL    12
Y       .FILL    8
Z       .BLKW    1             ; Holds X*Y at end


; Multiply subroutine
; Set R1 = R1 * R2 (if R2 >= 0); set R1 = 0 if Y < 0.
;
; Property:
;    Y  < 0 implies product = 0, k = Y
;    Y >= 0 implies product = X * (Y-k) and 0 <= k <= Y
;        (So when k = 0, product = X * Y)
;
; Register usage: R1 = X, R2 = Y; R3 = product, R4 = k
;
Mult     ST      R7, MU_R7      ; Save registers
         ST      R4, MU_R4
         ST      R3, MU_R3

; Establish product and k properties
;
         AND     R3, R3, 0      ; product = 0
         ADD     R4, R2, 0      ; counter k = Y

; If Y < 0, product = 0; otherwise, maintain
; product = X * (Y - k) while k = Y, Y-1, ....
; Stop when k = 0
;
MU_Loop  BRNZ    MU_Done        ; until k <= 0
         ADD     R3, R3, R1     ;   product += X
         ADD     R4, R4, -1     ;   k--
         BR      MU_Loop

; After loop, R3 = X * Y (if Y >= 0) or 0 (if Y < 0)
; Set R1 to result, restore registers, and return
;
MU_Done  ADD     R1, R3, 0      ; R1 = X * Y
         LD      R3, MU_R3      ; Restore registers
         LD      R4, MU_R4
         LD      R7, MU_R7
         JMP     R7             ; return to caller

; Save area for registers
;
MU_R1   .BLKW   1
MU_R3   .BLKW   1
MU_R4   .BLKW   1
MU_R7   .BLKW   1

        .END
