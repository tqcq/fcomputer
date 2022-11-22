        li      $1,0  ; x = 0  [0,31)
        li      $2,0  ; y = 0  [0,31)
        li      $6,0
        li      $10,1 ; always is one
        sw      $0,0($0) ; screen x
        sw      $0,1($0) ; screen y
        sw      $0,2($0) ; screen flash counter
loop:
        dec     $1       ; fix offset_x
        dec     $2       ; fix offset_y
        rdrand  $3
        modi    $3,$3,9
        divi    $4,$3,3  ; offset_x + 1
        modi    $5,$3,3  ; offset_y + 1

        inc     $6

        add     $1,$1,$4
        add     $2,$2,$5

        test    $1,31
        jle     L1
        subi    $1,$1,1
L1:
        test    $1,0
        jge     L2
        addi    $1,$1,1
L2:
        test    $2,31
        jle     L3
        subi    $2,$2,1
L3:
        test    $2,0
        jge     L4
        addi    $2,$2,1
L4:
        ; shl     $4,$10,$1
        ; shl     $5,$10,$2
        sw      $1,0($0)
        sw      $2,1($0)
        sw      $6,2($0)
        jmp loop
