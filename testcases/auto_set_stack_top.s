;ax l, bx r
mov ax,0
; 10 MB
mov bx,10485760


l1:
  test ax, bx
  je set_sp
  add cx,ax,bx
  div cx,cx,2
  mov dx,rand
  str [cx],dx
  ldr r4,[cx]
  test dx,r4
  je to_r

to_l:
  mov bx,cx
  jmp l1

to_r:
  add ax,cx,1
  jmp l1

set_sp:
  mov sp,bx

