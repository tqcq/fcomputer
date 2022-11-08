  mov bx,0
  mov ax,1
  mov cx,2

  mov dx,0
  jmp check_less
  mov dx,1

check_less:
  mov dx,2
  test bx,ax
  jl   check_equal
check_equal:
  mov dx,3
  test ax,ax
  je check_greater
check_greater:
  mov dx,4
  test cx,ax
  jg success
hlt:
  mov dx,1
  jmp hlt
success:
  mov dx,5
