mov sp,2032
jmp main

main:
    mov bx,10
    call function1
    jmp hlt

function1:
    mov bx,1
    ret

hlt:
        jmp hlt

