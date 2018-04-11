; CS 350, Fall 2016
;
; multiply.asm
;
; Illinois Institute of Technology, (c) 2016, James Sasaki

; Set product = X * Y (where Y >= 0), using repeated addition.
; If Y < 0, we halt.
;
; Pseudocode:
;
; Property: product = X*(Y-k) and 0 <= k <= Y
;    (So when k = 0, product = X*Y)
;
;    product = 0               ; Initialize product and k
;    k = Y
;    until k <= 0
;       product += X
;       k--
;
; Register usage: R1 = k, R2 = X, R3 = product
;
        .ORIG    x3050
         LD      R2, X         ; R2 = X
         AND     R3, R3, 0     ; R3 = X * (Y-k)
         LD      R1, Y         ; k = Y
Loop     BRNZ    Done          ; until k <= 0
         ADD     R3, R3, R2    ;    R3 = R3 + X
         ADD     R1, R1, -1    ;    k--
         BR      Loop
Done     ST      R3, product   ; product = X*Y
         HALT

X       .FILL    12
Y       .FILL    8
product .BLKW    1             ; Holds X*Y at end
        .END
