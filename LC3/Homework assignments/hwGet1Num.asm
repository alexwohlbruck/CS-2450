        .orig x3000
        JSR GETNUM
        JSR GETNUM
        HALT
GETNUM  ST R7, SAVER7
        ST R2, SAVER2
GN_TOP  GETC
        LD R2, N48
        ADD R0, R0, R2
        BRN GN_TOP
        ADD R2, R0, #-9
        BRP GN_TOP
        LD R7, SAVER7
        LD R2, SAVER2
        RET
SAVER7  .fill   #0
SAVER2  .fill   #0
N48     .fill   #-48
.end