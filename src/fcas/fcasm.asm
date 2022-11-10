#bits 32

#subruledef reladdr
{
        {addr: i32} =>
        {
                reladdr = addr - $
                assert(reladdr >= -0x8000)
                assert(reladdr <= 0x7FFF)
                reladdr
        }
}

#subruledef register
{
        {imm: i32} => {
                              assert(imm >= 0)
                              assert(imm < 32)
                              imm
                      }
        zero => 0
        at   => 1
        gp   => 28
        sp   => 29
        fp   => 30
        ra   => 31
}

#subruledef r_opcode 
{
        ;; register type
        and	=>	0b000000
        or	=>	0b000001
        xor	=>	0b000010
        sla	=>	0b000011
        sra	=>	0b000100
        srl	=>	0b000101
        add	=>	0b001010
        sub	=>	0b001011
        mul	=>	0b001100
        div	=>	0b001101
        mod	=>	0b001110
        pow	=>	0b001111
}

#subruledef i_opcode {
        ;;
        addi	=>	0b011010
        subi	=>	0b011011
        muli	=>	0b011100
        divi	=>	0b011101
        modi	=>	0b011110
        powi	=>	0b011111
        ;; imm type
        andi	=>	0b010000
        ori	=>	0b010001
        xori	=>	0b010010
        slai	=>	0b010011
        srai	=>	0b010100
        srli	=>	0b010101
        ;;
        pop	=>	0b110100
        push	=>	0b110101
        lwin	=>	0b110110
        swin	=>	0b110111
        test	=>	0b111100
        lui	=>	0b111101
        ;;
}

branch_opcode = 0b111110
zero_reg = 0b00000
#subruledef b_opcode {
        ;;              releative (base pc) @ less,equal,greater flag
        jl      =>	0b1 @ 0b100 
        je      =>	0b1 @ 0b010
        jg      =>	0b1 @ 0b001
        jle     =>	0b1 @ 0b110
        jge     =>	0b1 @ 0b011
        jne     =>	0b1 @ 0b101
}

#subruledef j_opcode {
        j	=>	0b111111
}

#ruledef
{
        ;; op $0,$1,$2     $0 <= $1 op $2
        {op : r_opcode} ${r0: register},${r1: register},${r2: register}  => 0b0000 @ 0b0000 @ 0b000 @ r2`5 @ r1`5 @ r0`5 @ op`6 @ asm { nop4 }

        ;; op $0,11($1)    $0 <= $1 op imm
        {op : i_opcode} ${r0: register},{imm:i16}(${r1:register}) => imm`16 @ r1`5 @ r0`5 @ op`6 @ asm { nop4 }
        {op : i_opcode} ${r0: register},${r1:register},{imm:i16} => imm`16 @ r1`5 @ r0`5 @ op`6  @ asm { nop4 }

        ;; branch 
        {op : b_opcode} {imm: i16} => imm`16 @ zero_reg @ op`5 @ branch_opcode ` 6

        ;; jmp
        j {imm : reladdr} => imm`16 @ 0b00000 @ 0b00000 @ 0b111111
        jmp {imm : reladdr} => imm`16 @ 0b00000 @ 0b00000 @ 0b111111

}

; pseudo instruction
#ruledef {
        move ${r1: register}, ${r2: register} => 0x0000 @ r2`5     @ r1`5 @ 0b011010 @ asm { nop3 }
        move ${r1: register}, {imm: i16}      => imm`16 @ zero_reg @ r1`5 @ 0b011010 @ asm { nop3 }

        lui  ${r1: register}, {imm: i16}      => imm`16 @ zero_reg @ r1`5 @ 0b011010 @ asm { nop3 }
        lui  ${r1: register}, %hi({imm: i32})    => (imm>>16)`16 @ zero_reg @ r1`5 @ 0b011010 @ asm { nop3 }
        lui  ${r1: register}, %lo({imm: i32})    => imm`16 @ zero_reg @ r1`5 @ 0b011010 @ asm { nop3 }

        li  ${r1: register}, {imm: i16}       => imm`16 @ r1 @ r1`5 @ 0b011010 @ asm { nop3 }
        li  ${r1: register}, %hi({imm: i32})  => (imm>>16)`16 @ r1 @ r1`5 @ 0b011010 @ asm { nop3 }
        li  ${r1: register}, %lo({imm: i32})  => imm`16 @ r1 @ r1`5 @ 0b011010 @ asm { nop3 }

        inc ${r1: register} => asm { addi $ r1,$ r1,1 }
        dec ${r1: register} => asm { addi $ r1,$ r1,-1 }

        test ${r1: register}, ${r2: register}  => asm { test $ r1, 0($ r2) }
        test ${r1: register}, {imm: i16}  => asm { test $ r1, imm($0) }

        ;; push $1
        push ${r: register} => asm { 
                push $ r,-1($29) 
                nop7
        }
        ;; pop $1
        pop  ${r: register} => asm { 
                pop $ r,0($29)
                nop7
        }

        lw ${r1: register},{imm: i16}(${r2: register}) => asm {
                lwin $ r1,imm($ r2)
                nop7
        }

        sw ${r1: register},{imm: i16}(${r2: register}) => asm {
                swin $ r1,imm($ r2)
                nop7
        }

        nop => 0xFFE00000 ; add $0,$0,$0
        hlt => 0x0000003F ; j 0
        ;; nop3 for move
        nop3 => 0xFFE00000 @ 0xFFE00000 @ 0xFFE00000
        ;; nop4 for calc
        nop4 => 0x0001003f
        ;; nop7 for lw, sw, push, pop
        nop7 => asm { 
                nop3
                nop4
        }
}
; # add $0,$0,$0

fcasm_init_system:
        move $1,0x7FFF
        move $sp,0
fcasm_system_find_stack_top:
        test $sp,$1
        je   fcasm_system_boot
        add  $2,$1,$sp
        divi $2,$2,2
        sw   $2,0($2)
        lw   $4,0($2)
        test $2,$4
        jne  fcasm_system_find_stack_top_mem_less_mid
fcasm_system_find_stack_top_mem_greate_mid:
        addi  $sp,$2,1
        j fcasm_system_find_stack_top
fcasm_system_find_stack_top_mem_less_mid:        
        move $1,$2
        j fcasm_system_find_stack_top

fcasm_system_boot:
        nop
