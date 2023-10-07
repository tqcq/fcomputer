_system_set_stack:
        ;; mid -> $t1
        li $t1, 0x3FFFFFFF
        ;; l -> $t2
        li $t2, 0
        ;; r -> $t0
        li $t0, 0x7FFFFFFF
        nop3
        j _system_set_stack_loop

_system_set_stack_loop_set_l:
        ; l = mid + 1
        addi $t2, $t1, 1
        nop3
_system_set_stack_loop:
        rdrand $v0
        add $t1, $t2, $t0
        test $t2, $t0, 0
        nop2
        shri $t1, $t1, 1
        nop
        jge _system_set_stack_loop_end
        nop2
        sw $v0, 0($t1)
        nop4
        lw $v1, 0($t1)
        nop6
        test $v1, $v0, 0
        nop4
        je _system_set_stack_loop_set_l
        ; r = mid
        mv $t0, $t1
        j _system_set_stack_loop
_system_set_stack_loop_end:
        addi $v0, $t2, -1
        jr $ra
