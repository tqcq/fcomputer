        test    $0,$0
        li      $3,1
        li      $4,1
loop:
        test    $3,20
        inc     $3
        jal     fun
        jl      loop

hlt:
        jmp     hlt
fun:
        inc     $4
        jr      $ra
