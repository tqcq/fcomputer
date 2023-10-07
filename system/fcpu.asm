#const start_addr = 0x0F7006C0
#bankdef program {
        #addr start_addr
        #size 0x8000
        #outp 0
        #labelalign 32
        #bits 32
}

#const op_prefix_11_one  = 0b11111111111
#const op_prefix_11_zero = 0b00000000000
#const op_prefix_16_zero = 0x0000
#const op_prefix_16_one  = 0xFFFF

#const zero_reg = 0b00000
#const sp_reg   = 0b11101

#ruledef nop_rule
{
        nop   => 0xFFFF @ 0b00000 @ 0b00000 @ 0b110011
        nop2  => asm { 
                nop
                nop
        }
        nop3  => asm { 
                nop 
                nop2 
        }
        nop4  => asm { jmp_next_line }
        nop5  => asm { 
                jmp_next_line
                nop
        }
        nop6  => 0x0001 @ zero_reg @ 0b01111 @ 0b111110
        nop7  => asm { 
                nop6
                nop
        }
}

#subruledef register
{
        $zero => 0b00000
        $at   => 0b00001
        $v0   => 0b00010
        $v1   => 0b00011
        $a0   => 0b00100
        $a1   => 0b00101
        $a2   => 0b00110
        $a3   => 0b00111
        $t0   => 0b01000
        $t1   => 0b01001
        $t2   => 0b01010
        $t3   => 0b01011
        $t4   => 0b01100
        $t5   => 0b01101
        $t6   => 0b01110
        $t7   => 0b01111
        $s0   => 0b10000
        $s1   => 0b10001
        $s2   => 0b10010
        $s3   => 0b10011
        $s4   => 0b10100
        $s5   => 0b10101
        $s6   => 0b10110
        $s7   => 0b10111
        $t8   => 0b11000
        $t9   => 0b11001
        $k0   => 0b11010
        $k1   => 0b11011
        $gp   => 0b11100
        $sp   => 0b11101
        $fp   => 0b11110
        $ra   => 0b11111
}

#fn hi(value) => value[31:16] @ 0x0000
#fn lo(value) => value[15:0]



#subruledef source 
{
        imm[{val: i16}]  => val
        uimm[{val: u16}] => val
}

#subruledef address 
{
        {addr} => {
                reladdr = addr - $
                assert(reladdr < 0x8000)
                assert(reladdr >= -0x8000)
                reladdr`16 @ zero_reg
        }

        {imm: i16}({reg: register}) => {
                imm @ reg
        }
}

#ruledef base_op
{
        and {dst:register}, {src:register}, {param:register} => op_prefix_11_one @ param @ src @ dst @ 0b000000
        or  {dst:register}, {src:register}, {param:register} => op_prefix_11_one @ param @ src @ dst @ 0b000001
        xor {dst:register}, {src:register}, {param:register} => op_prefix_11_one @ param @ src @ dst @ 0b000010
        sal {dst:register}, {src:register}, {param:register} => op_prefix_11_one @ param @ src @ dst @ 0b000011
        sar {dst:register}, {src:register}, {param:register} => op_prefix_11_one @ param @ src @ dst @ 0b000100
        shr {dst:register}, {src:register}, {param:register} => op_prefix_11_one @ param @ src @ dst @ 0b000101

        add {dst:register}, {src:register}, {param:register} => op_prefix_11_one @ param @ src @ dst @ 0b001010
        sub {dst:register}, {src:register}, {param:register} => op_prefix_11_one @ param @ src @ dst @ 0b001011
        mul {dst:register}, {src:register}, {param:register} => op_prefix_11_one @ param @ src @ dst @ 0b001100
        div {dst:register}, {src:register}, {param:register} => op_prefix_11_one @ param @ src @ dst @ 0b001101
        mod {dst:register}, {src:register}, {param:register} => op_prefix_11_one @ param @ src @ dst @ 0b001110
        pow {dst:register}, {src:register}, {param:register} => op_prefix_11_one @ param @ src @ dst @ 0b001111
}


#ruledef base_opi
{
        andi {dst: register}, {src: register}, {uimm: u16} => uimm @ src @ dst @ 0b010000
        ori  {dst: register}, {src: register}, {uimm: u16} => uimm @ src @ dst @ 0b010001
        xori {dst: register}, {src: register}, {uimm: u16} => uimm @ src @ dst @ 0b010010
        sali {dst: register}, {src: register}, {uimm: u16} => uimm @ src @ dst @ 0b010011
        sari {dst: register}, {src: register}, {uimm: u16} => uimm @ src @ dst @ 0b010100
        shri {dst: register}, {src: register}, {uimm: u16} => uimm @ src @ dst @ 0b010101

        addi {dst: register}, {src: register}, {imm: i16}  =>  imm @ src @ dst @ 0b011010
        subi {dst: register}, {src: register}, {imm: i16}  =>  imm @ src @ dst @ 0b011011
        muli {dst: register}, {src: register}, {imm: i16}  =>  imm @ src @ dst @ 0b011100
        divi {dst: register}, {src: register}, {imm: i16}  =>  imm @ src @ dst @ 0b011101
        modi {dst: register}, {src: register}, {imm: i16}  =>  imm @ src @ dst @ 0b011110
        powi {dst: register}, {src: register}, {imm: i16}  =>  imm @ src @ dst @ 0b011111

        inc {r: register} => asm { addi {r}, {r}, 1 }
        dec {r: register} => asm { addi {r}, {r}, -1 }
}

#ruledef complex_op
{
        pop     {dst: register}                  => 0x0001 @ sp_reg   @ dst @ 0b110100
        push    {dst: register}                  => op_prefix_16_zero @ sp_reg   @ dst @ 0b110101
        lw      {dst: register}, {addr: address} => addr                         @ dst @ 0b110110
        sw      {dst: register}, {addr: address} => addr                         @ dst @ 0b110111

        rdrand  {dst: register}                  => op_prefix_16_zero @ zero_reg @ dst @ 0b111000
        rdtsc   {dst: register}                  => op_prefix_16_zero @ zero_reg @ dst @ 0b111001

        ;; store current pc+offset
        store_pc {dst: register}, {offset: i16}   => offset            @ zero_reg @ dst @ 0b111010

        test    {dst: register}, {src: register}, {imm: i16} => imm   @ src      @ dst @ 0b111100
        lui     {dst: register}, {imm: i32}      => imm[31:16]               @ zero_reg @ dst @ 0b111101

}

#ruledef jmp_op
{
        jl      {addr: address}   => addr @ 0b01100 @ 0b111110
        je      {addr: address}   => addr @ 0b01010 @ 0b111110
        jg      {addr: address}   => addr @ 0b01001 @ 0b111110
        jle     {addr: address}   => addr @ 0b01110 @ 0b111110
        jge     {addr: address}   => addr @ 0b01011 @ 0b111110
        jne     {addr: address}   => addr @ 0b01101 @ 0b111110

        jz      {addr: address}   => asm { je  addr }
        jnz     {addr: address}   => asm { jne addr }

        jmp_abs {imm: i16}({r: register}) => imm @ r @ 0b10111 @ 0b111110
        jmp_rel {imm: i16}({r: register}) => imm @ r @ 0b11111 @ 0b111110

        
        j {addr}                         => 
        {
                reladdr = addr - $
                high = reladdr & 0xFFFF0000
                assert((high == 0) || (high == 0xFFFF0000))
                reladdr`16 @ 0x003F
        }

        jmp_next_line => asm { jmp_rel 1($zero) }
}

#ruledef pusle_op
{

        mv   {dst: register}, {src: register} => asm { addi {dst}, {src}, 0 }

        li {dst: register}, {imm}      => 
        {
                assert((imm & 0xFFFF) != 0)
                assert((imm & 0xFFFF0000) != 0)
                asm { 
                        lui {dst}, {imm} 
                        nop3 
                        ; ori {dst}, {dst}, lo({imm}) 
                        addi {dst}, {dst}, lo({imm})
                }
        }
        li {dst: register}, {imm}      => {
                assert((imm & 0xFFFF) != 0)
                assert((imm & 0xFFFF0000) == 0)
                asm { addi {dst}, $zero, lo({imm}) }
        }

        li {dst: register}, {imm}      => {
                assert((imm & 0xFFFF) == 0)
                asm { lui {dst}, hi({imm}) }
        }

}

#ruledef 
{
        jalr {r: register}, {label} => asm 
        {
                nop7
                lui $at, hi({label})
                push {r} 
                store_pc {r}, 4
                nop
                addi $at,$at,lo({label})
                nop4
                jmp_abs 0($at)
                pop {r}
                nop7
        }


        jal {label} => asm 
        {
                jalr $ra, {label}
        }
                
        jr {r: register} => asm {
                jmp_abs 0({r})
        }

        ; hlt   => 0x0000003F
        hlt   => asm { jmp_abs 0($zero) }
}

#const state_clear_reg  = 0x1
#const state_set_stack  = 0x2
#const state_run_main   = 0x3
#const state_hlt        = 0x4

#bank program
system_start:
        li $sp,4143
        jal main
        hlt