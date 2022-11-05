mov ax,rand
mov cx,0

test_start:
  str [cx],ax
  ldr bx,[cx]
  inc cx
  test ax,bx
  mov ax,rand
  je test_start
display:
  sub cx,cx,1
  mov ax,cx
  mov bx,cx
  mov dx,cx
