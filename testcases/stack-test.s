mov ax, 1
mov sp,2032

l1:
    test sp,0
    jle  stackoverflow
    push  ax
    ldr   bx,[sp]
    test  ax,bx
    jne   hlt
    inc   ax
    jmp   l1


stackoverflow:
    mov sp,11
    jmp hlt
ne:
    mov sp 12
    jmp hlt
   
hlt:
    jmp hlt

