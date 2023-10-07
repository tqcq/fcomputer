#include "../system/fcpu.asm"
fib:
fib_init:
    li $k1,1 ;output

    li $t0,1
    li $t1,1
    li $t2,1
    li $t7,0x3FFFFFFF ; up limit
    ; display $k1
    ; step 1: $t0 -> f(n-2), $t1 -> f(n-1), $k1 -> f(n)
fib_loop:
    test $t2,$t7,0
    add $t2,$t0,$t1
    mv $t0,$t1
    mv $k1,$t1
    nop
    mv $t1,$t2
    jge fib_init
    j fib_loop

fib_ret:
    jr $ra

main:
    jal fib
    jr $ra