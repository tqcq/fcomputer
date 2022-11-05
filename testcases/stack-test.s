mov ax, rand
mov sp,2048

l1:
    test sp,0
    push  ax
    jle  stackoverflow
    ldr   bx,[sp]
    test  ax,bx
    mov   ax,rand
    je    l1

fail:
    mov dx 12
hlt:
    jmp hlt

stackoverflow:
    mov dx,11
    jmp hlt


