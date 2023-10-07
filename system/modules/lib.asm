; $a0 = dst, $a1=src, $a2=len
memcpy:
    li $k0, 11
    test $a2, $zero,8
    nop4

memcpy_8_mv:
    jl memcpy_mv
    addi $a2,$a2,-8

;; load 8 bytes
    lw $t0, 0($a1)
    lw $t1, 1($a1)
    lw $t2, 2($a1)
    lw $t3, 3($a1)
    lw $t4, 4($a1)
    lw $t5, 5($a1)
    lw $t6, 6($a1)
    lw $t7, 7($a1)

;; write 8 bytes
    sw $t0, 0($a0)
    sw $t1, 1($a0)
    sw $t2, 2($a0)
    sw $t3, 3($a0)
    sw $t4, 4($a0)
    sw $t5, 5($a0)
    sw $t6, 6($a0)
    sw $t7, 7($a0)

    test $a2,$zero,8
    addi $a1,$a1,8
    addi $a0,$a0,8
    j memcpy_8_mv

memcpy_mv:
    test $a2,$zero,0
    lw $t0, 0($a1)
    dec $a2
    nop2
    jle memcpy_ret
    inc $a0
    inc $a1
    sw $t0, 0($a0)
    j memcpy_mv

memcpy_ret:
    jr $ra