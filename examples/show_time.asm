#include "system/fcpu.asm"
main:
        ; $t4 * 1000000 + $t1 * 10000 + $t2 * 100 + $t6
        li $t7,1000000
        li $t6,86400
        rdtsc $at
        nop3
main_loop:
        ; $t1 -> hour, $t2 -> minute, $t0 -> second 
        ; $t4 -> day
        ; $k0 = $t1 * 1000000 + $t2 * 1000 + $t3
        divi $t1,$at,3600  ; hour need mod
        divi $t2,$at,60    ; minute need mod
        modi $k0,$at,60    ; second
        div $t4,$at,$t6    ; day
        rdtsc $at
        modi $t1,$t1,24    ; hour
        modi $t2,$t2,60    ; minute
        nop
        mul $t4, $t4, $t7  ; day * 1000000
        nop
        muli $t1, $t1, 10000; hour * 10000
        muli $t2, $t2, 100  ; minute * 100
        nop
        add $k0, $k0, $t4 ; day * 1000000 + second
        nop3
        add $k0, $k0, $t1 ; day * 1000000 + hour * 10000 + second
        nop3
        add $k0, $k0, $t2 ; day * 1000000 + hour * 10000 + minute * 100 + second
        nop3
        mv $k1, $k0
        j main_loop
        jr $ra
