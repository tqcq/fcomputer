init:
  mov ax,0
  mov bx,1
  mov cx,2
  mov dx,1000000000
  mov r10,0

l1:
  add ax,bx,cx
  test ax,dx
  mov  r10,ax
  jg init
  add bx,ax,cx
  test bx,dx
  mov  r10,bx
  jg init
  add cx,ax,bx
  test cx,dx
  mov  r10,cx
  jg init
  jmp l1




