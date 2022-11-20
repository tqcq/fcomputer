        li      $2,-1
loop:
        rdrand  $1
        inc     $2
        sw      $1,0($2)
        lw      $3,0($2)
        jmp     loop
        inc     $2
