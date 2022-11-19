#include "fcasm_auto_nop.asm"
fcasm_init_system:
        move $1,0x7FFF
        move $sp,0
fcasm_system_find_stack_top:
        test $sp,$1
        je   fcasm_system_boot
        add  $2,$1,$sp
        divi $2,$2,2
        sw   $2,0($2)
        lw   $4,0($2)
        test $2,$4
        jne  fcasm_system_find_stack_top_mem_less_mid
fcasm_system_find_stack_top_mem_greate_mid:
        addi  $sp,$2,1
        j fcasm_system_find_stack_top
fcasm_system_find_stack_top_mem_less_mid:
        move $1,$2
        j fcasm_system_find_stack_top

fcasm_system_boot:
        move $1,0

test_stack:
        inc $1
        push $1
        test $1,11
        jl test_stack
