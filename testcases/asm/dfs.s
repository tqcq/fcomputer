mov ax,0
call fun1
jmp hlt

fun1:
  test ax,10
  jge fun1_ret
  inc ax
  call fun1
  dec ax
fun1_ret:
  ret

hlt:
  jmp hlt
