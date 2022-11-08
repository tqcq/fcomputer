mov ax,0
mov dx,2000
jmp main

main:
    inc ax
    call function3
    dec ax
    jmp hlt

function1:
    inc ax
    call function3
    dec ax
    ret

function2:
    inc ax
    call function1
    dec ax
    ret

function3:
    inc ax
    test ax,dx
    jg skip_call
    call function2
skip_call:
    dec ax
    ret

hlt:
    mov bx,ax
    jmp hlt
