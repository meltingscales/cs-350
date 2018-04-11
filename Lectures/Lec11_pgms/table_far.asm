; CS 350, Fall 2016
;
; table_far.asm
;
; Illinois Institute of Technology, (c) 2016, James Sasaki

; Set table[k] = x where k and k are variables and
; the table is not with PC-offset distance.
;
; We assume table entries take up one word each
;
; Register usage: R0 = &table[0], R1 = &table[k]. R2: temp
;

; To make R0 = &table[0], we load the address of the table,
; as stored in tablePtr.
         .ORIG  x8000
          LD    R0, tablePtr    ; R0 = tablePtr = &table[0]

; Make R2 = &table[k]
          LD    R2, k           ; R2 = k
          ADD   R1, R0, R2      ; R1 = &table[k]

; Set table[k] = x
          LD    R2, x           ; R2 = value
          STR   R2, R1, 0       ; *R1 (= table[k]) = value
          HALT

k        .FILL  4               ; index into table
x        .FILL  -1              ; value to copy into table

; Define tablePtr = &table[0], then declare some space to
; ensure that the table is not PC-offset-accessible.  Note
; table starts at x8109.
;
tablePtr .FILL  table           ; &table[0]
         .BLKW  256
table    .BLKW  100             ; space for table[0..99]
         .END
