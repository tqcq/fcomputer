#include "fcasm_auto_nop.asm"

#ruledef {
    check => asm {
        jne loop
        inc $1
    }
}

test_start:
    move $1,1
;; 1  li   test
    li   $2,11
    test $2,11
    check

;; 2  and test
    andi $2,$2,10
    test $2,10
    check
;; 3 or test
    ori $2,$2,0xFFF
    test $2,0xFFF
    check
;; 4 xor test
    xori $2,$2,0xF
    test $2,0xFF0
    check
;; 5 slai test
    slai $2,$2,24
    test $2,0
    jge loop
    inc $1
;; 6 srai test
    srai $2,$2,16
    test $2,0
    jge loop
    inc $1
;; 7 srli test
    srli $2,$2,1
    test $2,0
    jle  loop
    inc $1

;; 8 add
    addi $2,$0,99
    addi $2,$2,131
    test $2,99+131
    check
;; 9 sub $2 = 230
    subi $2,$2,77
    test $2,99+131-77
    check
;;  10 div $2 = 153
    divi $2,$2,11
    test $2,13
    check
;;  11 mod =
    modi $2,$2,7
    test $2,6
    check
;;  12 pow $2 = 6
    powi $2,$2,3
    test $2,216
    check

;;  13 jne
    move $7,0
jne_test_loop:
    inc $7
    test $7,13
    jne jne_test_loop
    test $7,13
    check

;; 14 load_save test
    move $2,1314
    sw $2,0($2)
    lw $4,0($2)
    test $2,$4
    check

;; 15 mem_copy test
    sw $0,0($0)
    move $2,1
mem_copy_test_start:
    test $2,10
    jg mem_copy_end
    lw $3,0($2)
    inc $3
    sw $3,0($2)
    inc $2
    jmp mem_copy_test_start
mem_copy_end:
    test $0,$0
    check

;; 16 mem_test
   move $2,0
   move $3,1
mem_test_loop:
   sw $3,0($2)
   lw $4,0($2)
   test $3,$4
   check
   inc $2
   inc $3
   jmp mem_test_loop

   inc $1

loop:
    addi $1,$1,100
    hlt