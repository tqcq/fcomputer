;ax l, bx r
mov ax,0
; 10 MB
mov bx,1
mov dx,0

_system_l1:
  mul bx,bx,3
  inc dx
  str [bx],dx
  ldr r10,[bx]
  test r10,dx
  je _system_l1

_system_l2:
  add cx,ax,bx
  inc dx
  div cx,cx,2
  test ax,bx
  je _system_set_sp
  str [cx],dx
  ldr r10,[cx]
  test r10,dx
  je _system_to_r

_system_to_l:
  mov bx,cx
  jmp _system_l2

_system_to_r:
  add ax,cx,1
  jmp _system_l2

_system_set_sp:
  mov sp,bx

