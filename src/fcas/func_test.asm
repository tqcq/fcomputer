#include "fcasm_auto_nop.asm"
move $sp,2048

program_start:
    move $1,0
    move $4,11
    call fib
    jmp program_end

fib:
    test $4,1
    jl fib_ret_0
    je fib_ret_1
    dec $4
    push $4
    call fib
    pop $4
    push $2

    dec $4
    call fib
    pop $24
    add $2,$2,$24
    ret

fib_ret_0:
    move $2,0
    ret
fib_ret_1:
    move $2,1
    ret

program_end:
    hlt