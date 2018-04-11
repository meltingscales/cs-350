; CS 350, Fall 2016
;
; table_near.asm
;
; Illinois Institute of Technology, (c) 2016, James Sasaki

; Set table[k] = x where k and k are variables and
; the table is with PC-offset distance.
;
; We assume table entries take up one word each
;
; Register usage: R0 = &table[0], R1 = &table[k]. R2: temp
;

; Since the table is within a PC-offset distance, we
; can use LEA to get &table[0].
         .ORIG  x8000
          LEA   R0, table       ; R0 = &table[0]

; Make R2 = &table[k]
          LD    R2, k           ; R2 = k
          ADD   R1, R0, R2      ; R1 = &table[k]

; Set table[k] = x
          LD    R2, x           ; R2 = value
          STR   R2, R1, 0       ; *R1 (= table[k]) = value
          HALT

k        .FILL  4               ; index into table
x        .FILL  -1              ; value to copy into table

table    .BLKW  100             ; space for table[0..99]
         .END
