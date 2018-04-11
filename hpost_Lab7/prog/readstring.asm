; CS 350, Fall 2016
;
; readstring.asm
;
; Illinois Institute of Technology, (c) 2016, James Sasaki

; Read and echo characters until we see a return. (Also echo
; the return.)  Store the characters (but not the return) as
; a string in a buffer.
;
; Pseudocode 1:
;    Point to the beginning of the buffer
;    Prompt user for the input
;    Read a character
;    until character = return
;       Echo the character
;       Copy the char to the pointed position in the buffer
;       Point to the next buffer position
;       Read the next character
;    Echo the return character
;    End the string in the buffer and print it
;
; Pseudocode 2:
;    char buffer[…], *bp; // bp = buffer position
;    bp = &buffer[0];
;    Print "Enter chars (return to halt): "
;    Read char into R0
;    Calculate R0 - return char
;    until R0 - return char == 0
;       Print char in R0
;       *bp = R0
;       ++bp
;       Read char into R0
;       Calculate R0 - return char
;    end loop
;    Print the return character
;    *bp = '\0' (end the string)
;    Print the string
;    Halt
;
; Register usage
;    R0 = GETC/OUT char, R1 = bp (buffer position)
;    R2 = -(return char), R3 = temp
;
        .ORIG    xC000
         LEA     R1, buffer    ; bp = &buffer
         LEA     R0, msg
         PUTS                  ; prompt for input
         GETC                  ; get char into R0
         LD      R2, retChar   ; R2 = return char
         NOT     R2, R2        ; R2 = -(return char) - 1
         ADD     R2, R2, 1     ; R2 = -(return char)
         ADD     R3, R0, R2    ; calculate R0 - return char

Loop     BRZ     Done          ; until r0 = return char
         OUT                   ;    print char in R0
         STR     R0, R1, 0     ;    *bp = char read in
         ADD     R1, R1, 1     ;    ++bp
         GETC                  ;    get char into R0
         ADD     R3, R0, R2    ;    calc char - return char
         BR      Loop          ; continue loop

Done     OUT                   ; print return char in R0
         AND     R3, R3, 0     ; R3 = null char ('\0')
         STR     R3, R1, 0     ; *bp = null char
         LEA     R0, buffer
         PUTS                  ; print the string we read in
         HALT

retChar .STRINGZ "\n"          ; Return character (\n)
msg     .STRINGZ "Enter chars (return to halt): "
buffer  .BLKW    80            ; buffer space for string
        .END