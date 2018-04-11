; CS 350, Fall 2016
;
; read_subr.asm
;
; Illinois Institute of Technology, (c) 2016, James Sasaki

; Example of subroutine; uses JSR/RET call and return,
; saves registers with the program.

; The main program exercises the ReadLine subroutine.
;
            .ORIG   x3000
             LEA    R0, msg1        ; read first message
             JSR    ReadLine

             LD     R0, addr_msg2   ; read second message
             LD     R7, addr_read   ;
             JSRR   R7
             HALT

; Buffers to read lines into
;
msg1        .BLKW   100
msg2        .BLKW   100

; Pointers &ReadLine and &msg2 to illustrate JSRR
;
addr_read   .FILL   ReadLine
addr_msg2   .FILL   msg2

; ReadLine: Read a return-terminated string into a
; buffer pointed to by R0.  Uses the same pseudocode as in
; readstring.asm.  Registers are restored before return.
;
; Register usage:
;    R0 = GETC/OUT char,  char *R1 = bp (buffer position),
;    R2 = -(return char), R3 = temporary
;
ReadLine     ST     R0, RS_R0   ; Save R0
             ST     R1, RS_R1   ; Save R1
             ST     R2, RS_R2   ; Save R2
             ST     R3, RS_R3   ; Save R3
             ST     R7, RS_R7   ; Save R7

; Calculate R2 = -(ASCII char for return)
             LD     R2, RS_rc   ; R2 = return char
             NOT    R2, R2      ; R2 = -(return char) - 1
             ADD    R2, R2, 1   ; R2 = -(return char)

; Initialize bp = &buffer, prompt user for string,
; and read first char
;
             ADD    R1, R0, 0   ; bp = &buffer
             LEA    R0, RS_msg  ; get prompt message
             PUTS               ; prompt for input
             GETC               ; read char into R0
             ADD    R3, R0, R2  ; calculate R0 - return char

; Repeat printing the char, storing it, and reading the next
; char until we read in a return char
;
RS_Loop      BRZ    RS_Done     ; until char read = return
             OUT                ;     print char read in
             STR    R0, R1, 0   ;     *bp = char read in
             ADD    R1, R1, 1   ;     bp++
             GETC               ;     read next char
             ADD    R3, R0, R2  ;     calc char - return char
             BR     RS_Loop     ; continue loop

; When we see the end of the line, terminate the string
; in the buffer and print it out
;
RS_Done      OUT                ; echo the return char in R0
             AND    R3, R3, 0   ; get a null char ('\0')
             STR    R3, R1, 0   ; terminate buffer string
             LD     R0, RS_R0   ; point to buffer
             PUTS               ; print the string we read in
             LD     R0, RS_rc   ; get a newline
             OUT                ; end this line of output

; Restore the registers and return
;
             LD     R7, RS_R7
             LD     R3, RS_R3
             LD     R2, RS_R2
             LD     R1, RS_R1
             LD     R0, RS_R0
             JMP    R7

RS_rc      .STRINGZ "\n"        ; ASCII newline char
RS_msg     .STRINGZ "Enter chars (then return): "

; Save area for registers
RS_R0       .BLKW   1
RS_R1       .BLKW   1
RS_R2       .BLKW   1
RS_R3       .BLKW   1
RS_R7       .BLKW   1
            .END
