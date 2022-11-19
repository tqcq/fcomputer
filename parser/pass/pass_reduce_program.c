/**
 * Created by Tqcq Ann on 11/19/22.
**/

#include "pass_reduce_program.h"

static InstructionPtr* FindNULL(InstructionPtr* begin, InstructionPtr* end, int len)
{
    InstructionPtr *old_begin = begin;
    if (end - begin < len) {
        return end;
    }

    while (len > 0 && begin < end && *begin == NULL) {
        ++begin;
        --len;
    }

    if (len == 0)
        return old_begin;

    return end;
}

static void ReplaceInstruction(Program* program, InstructionPtr* begin, InstructionPtr instruction, int len)
{
    *begin++ = instruction;
    InstructionPtr *p = begin + len - 1;
    while (p < program->instruction_list + program->len) {
        *begin++ = *p++;
    }
    program->len -= len - 1;
}

void PassReduceProgram(Program* program)
{
    static struct Operand* zero_reg;
    static struct Operand* one_imm;
    static struct Operand* goto_flag;
    static struct Instruction nop4;
    static struct Instruction nop6;

    zero_reg = ToOperand(OPERAND_REG, 0, 0, "$zero");
    one_imm = ToOperand(OPERAND_IMM, 0, 0, "1");
    goto_flag = ToOperand(OPERAND_REG, 0, 0, "$31");
    nop4 = (struct Instruction) {I_RI, "jmp", *zero_reg, *zero_reg, *one_imm};
    nop6 = (struct Instruction) {I_RI, "goto",*goto_flag, *zero_reg, *one_imm};

    if (!program)
        return;

    for (int i = 0; i < program->len; ++i) {
        InstructionPtr *begin = program->instruction_list + i;
        InstructionPtr *end = program->instruction_list + program->len;

        InstructionPtr* nop6_pos = FindNULL(begin, end, 6);
        if (nop6_pos != end) {
            ReplaceInstruction(program, begin, &nop6, 6);
            continue;
        }

        InstructionPtr* nop4_pos = FindNULL(begin, end, 4);
        if (nop4_pos != end) {
            ReplaceInstruction(program, begin, &nop4, 4);
            continue;
        }
    }
}
