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
        ;; and	=>	0b000000
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
        ;; addi	=>	0b011010
        subi	=>	0b011011
        muli	=>	0b011100
        divi	=>	0b011101
        modi	=>	0b011110
        powi	=>	0b011111
        ;; imm type
        andi	=>	0b010000
        ori	    =>	0b010001
        xori	=>	0b010010
        slai	=>	0b010011
        srai	=>	0b010100
        srli	=>	0b010101
        ;;
        pop	=>	0b110100
        push	=>	0b110101
        lwin	=>	0b110110
        swin	=>	0b110111

        push_pc => 0b111010

        test	=>	0b111100
        lui	    =>	0b111101
        ;;
}

branch_opcode = 0b111110
zero_reg = 0b00000
#subruledef b_opcode {
        ;;              releative (ase pc) @ less,equal,greater flag
        jl      =>	0b01 @ 0b100
        je      =>	0b01 @ 0b010
        jg      =>	0b01 @ 0b001
        jle     =>	0b01 @ 0b110
        jge     =>	0b01 @ 0b011
        jne     =>	0b01 @ 0b101
}

#subruledef j_opcode {
        j	=>	0b111111
}

#ruledef
{
        add ${r0: register},${r1: register},${r2: register}  => 0b0000 @ 0b0000 @ 0b000 @ r2`5 @ r1`5 @ r0`5 @ 0b000000 @ asm { nop3 }
        ;; op $0,$1,$2     $0 <= $1 op $2
        {op : r_opcode} ${r0: register},${r1: register},${r2: register}  => 0b0000 @ 0b0000 @ 0b000 @ r2`5 @ r1`5 @ r0`5 @ op`6 @ asm { nop4 }

        ;; op $0,11($1)    $0 <= $1 op imm
        addi ${r0: register},${r1:register},{imm:i16} => imm`16 @ r1`5 @ r0`5 @ 0b011010 @ asm { nop3 }
        {op : i_opcode} ${r0: register},{imm:i16}(${r1:register}) => imm`16 @ r1`5 @ r0`5 @ op`6 @ asm { nop4 }
        {op : i_opcode} ${r0: register},${r1:register},{imm:i16} => imm`16 @ r1`5 @ r0`5 @ op`6  @ asm { nop4 }

        ;; branch 
        {op : b_opcode} {imm: reladdr} => imm`16 @ zero_reg @ op`5 @ branch_opcode ` 6

        ;; jmp
        jmp {imm : reladdr} => imm`16 @ 0b00000 @ 0b00000 @ 0b111111
        jmp {imm: u32}(${r: register}) => asm {
            lui $31,%hi(imm)
            li  $31,%lo(imm)
            asm { imm`16 @ r`5 @ 0b10111 @ 0b111110 }
        }


}

; pseudo instruction
#ruledef {
        move ${r1: register}, ${r2: register} => 0x0000 @ r2`5     @ r1`5 @ 0b011010 @ asm { movenop }
        move ${r1: register}, {imm: i16}      => imm`16 @ zero_reg @ r1`5 @ 0b011010 @ asm { movenop }

        lui  ${r1: register}, {imm: i16}      => imm`16          @ zero_reg @ r1`5 @ 0b111101 @ asm { movenop }
        lui  ${r1: register}, %hi({imm: i32})    => (imm>>16)`16 @ zero_reg @ r1`5 @ 0b111101 @ asm { movenop }
        lui  ${r1: register}, %lo({imm: i32})    => imm`16       @ zero_reg @ r1`5 @ 0b111101 @ asm { nop3 }

        li  ${r1: register}, {imm: i16}       => imm`16          @ zero_reg @ r1`5 @ 0b011010 @ asm { movenop }
        li  ${r1: register}, %hi({imm: i32})  => (imm>>16)`16    @ zero_reg @ r1`5 @ 0b011010 @ asm { movenop}
        li  ${r1: register}, %lo({imm: i32})  => imm`16          @ zero_reg @ r1`5 @ 0b011010 @ asm { movenop }

        push_pc  ${r1: register},{imm:i16} => imm`16 @  0b00000 @ r1`5 @ 0b111010 ; pc start from 1, already is PC + 1 (pc [1,)  PC [0, ))
        jal_in {imm: reladdr} => imm`16 @ zero_reg @ 0b11111 @ 0b111110

        jal {imm: i16} => asm {
            push_pc $31,1
            jal_in imm
        }

        jr ${r1: register} => 0x0000 @ r1`5 @ 0b10111 @ 0b111110

        call {imm: u32} => asm {
            push_pc $31,6
            movenop
            push $31,-1($29)
            jal_in imm
        }

        ret => asm {
            pop $31
            jr $31
        }

        inc ${r1: register} => asm { addi $ r1,$ r1, 1 }
        dec ${r1: register} => asm { addi $ r1,$ r1,-1 }

        test ${r1: register}, ${r2: register}  => asm { test $ r1, 0($ r2) }
        test ${r1: register}, {imm: i16}  => asm { test $ r1, imm($0) }

        ;; push $1
        push ${r: register} => asm { 
                push $ r,-1($29)
                memwnop
        }
        ;; pop $1
        pop  ${r: register} => asm { 
                pop $ r,0($29)
                memrnop
        }

        lw ${r1: register},{imm: i16}(${r2: register}) => asm {
                lwin $ r1,imm($ r2)
                memrnop
        }

        sw ${r1: register},{imm: i16}(${r2: register}) => asm {
                swin $ r1,imm($ r2)
                memwnop
        }

        nop => 0xFFE00000 ; add $0,$0,$0
        hlt => 0x0000003F ; j 0

        nop2 => asm {nop} @ asm {nop}
        ;; nop3 for move
        nop3 => asm {nop} @ asm {nop2}
        ;; nop4 for calc
        nop4 => 0x0001003f
        nop5 => asm { nop } @ asm {nop4}
        ;; nop7 for lw, sw, push, pop
        nop6 => asm {
            nop
            nop5
        }
        nop7 => asm {
                nop2
                nop4
        }

        memrnop => asm { nop7 }
        memwnop => asm { nop5 }
        calcnop => asm { nop4 }
        movenop => asm { nop3 }
}

#bankdef program
{
    #addr 0x7000_0000
    #size 0x8000
    #outp 0
}
