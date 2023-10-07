#include "../system/fcpu.asm"

; n = $a0
fun:
    test $a0, $zero, 0
    nop4
    jle fun_ret
    dec $a0
    mv $k1, $a0
    jal fun
    inc $a0
fun_ret:
    mv $k1, $a0
    jr $ra

main:
    li $a0,20
    jal fun
    mv $k1,$a0
    jr $ra