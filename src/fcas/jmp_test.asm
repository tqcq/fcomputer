#include "fcasm.asm"

loop:
move $1,0
test $0,1
je loop
inc $1
kk:
   test $0,$0
   jne end
   inc $1
   inc $1
end:
   hlt