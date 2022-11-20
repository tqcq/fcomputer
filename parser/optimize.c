/**
 * Created by Tqcq Ann on 11/18/22.
**/

#include "optimize.h"
#include "opcode.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef NOMINMAX

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#endif /* NOMINMAX */

enum ARGS_INDEX {
    OPERAND_RD = 0,      // reg 0
    OPERAND_RT = 1,      // reg 1
    OPERAND_RS = 2,      // reg 2
    OPERAND_MEM = 3,     // reg 3
    OPERAND_LEG_FLAG = 4,// reg 4
};

typedef struct {
    char opname[10];
    int delay[5];

    /*
    int  rd_delay; // rw
    int  rt_delay; // rw
    int  rs_delay; // rw
    int  mem_read_delay;
    int  leg_flag_delay; // less, equal, greate
    */
} DelayCycle;

DelayCycle DelayCycleTable[] = {
    {"default",         5, 0, 0, 0, 0},

    {"add",             4, 0, 0, 0, 0},
    {"addi",            4, 0, 0, 0, 0},
    {"push",            0, 2, 0, 5, 0},
    {"pop",             7, 2, 0, 0, 0},
    {"sw",              0, 0, 0, 5, 0},
    {"lw",              7, 0, 0, 0, 0},
    {"test",            0, 0, 0, 0, 5},
};

DelayCycle *GetDelayCycle(const char *opname) {
    for (int i = 0; i < sizeof(DelayCycleTable) / sizeof(DelayCycle); i++) {
        if (!strcmp(opname, DelayCycleTable[i].opname)) {
            return DelayCycleTable + i;
        }
    }

    return GetDelayCycle("default");
}

static int GetMinDuration(const char *opname, enum ARGS_INDEX idx) {
    DelayCycle *delay_cycle = GetDelayCycle(opname);
    if (delay_cycle == NULL) {
        int len = strlen(opname);
        delay_cycle = GetDelayCycle("default");
    }

    if (!strcmp("jmp", opname) || opname[0] == 'j' || !strcmp("goto", opname)) {
        return 0;
    }

    return delay_cycle->delay[idx];
}

static int HasArg(const char *opname, enum ARGS_INDEX idx) {
    int opcode = GetOpcode(opname);

    if (opcode <= 15) {
        return 1;
    }

    if (opcode <= 61) {
        return idx < 2;
    }

    if (opcode == 62) {
        return idx == 1;
    }

    return 0;
}

static int Conflict(InstructionPtr before, int before_pc,
                    InstructionPtr after, int after_pc) {
    if (before->type == I_LABEL || after->type == I_INCLUDE)
        return 0;

    if (!before || !after) {
        return 0;
    }

    int after_regs[3] = {
            HasArg(after->opname, OPERAND_RD), 
            HasArg(after->opname, OPERAND_RT), 
            HasArg(after->opname, OPERAND_RS)
    };

    int before_opcode = GetOpcode(before->opname);
    int after_opcode = GetOpcode(after->opname);

    DelayCycle *delay_table = GetDelayCycle(before->opname);
    int delay_max = INT32_MAX;

    // for write rd
    if (HasArg(before->opname, OPERAND_RD) && delay_table->delay[OPERAND_RD] > 0) {
        int write_reg_idx = OperandToValue(before->args);
        if (write_reg_idx == 0)
                write_reg_idx = -1;

        if (after_regs[0] && write_reg_idx == OperandToValue(after->args))
            delay_max = min(delay_max, delay_table->delay[OPERAND_RD]);
        if (after_regs[1] && write_reg_idx == OperandToValue(after->args + 1))
            delay_max = min(delay_max, delay_table->delay[OPERAND_RD]);
        if (after_regs[2] && write_reg_idx == OperandToValue(after->args + 2))
            delay_max = min(delay_max, delay_table->delay[OPERAND_RD]);
    }
    // for sp
    if (!strcmp(before->opname, "push") || !strcmp(before->opname, "pop")) {
        int write_reg_idx = OperandToValue(before->args + 1);
        if (write_reg_idx == 0)
                write_reg_idx = -1;

        if (after_regs[0] && write_reg_idx == OperandToValue(after->args))
            delay_max = min(delay_max, 2);
        if (after_regs[1] && write_reg_idx == OperandToValue(after->args + 1))
            delay_max = min(delay_max, 2);
        if (after_regs[2] && write_reg_idx == OperandToValue(after->args + 2))
            delay_max = min(delay_max, 2);
    }


    if (before_opcode == 60 && after_opcode == 62) // branch
        delay_max = min(delay_max, delay_table->delay[OPERAND_LEG_FLAG]);

    if ((before_opcode == 53 && after_opcode == 52)
        || (before_opcode == 55 && after_opcode == 54))
        delay_max = min(delay_max, delay_table->delay[OPERAND_MEM]);

    if (delay_max == INT32_MAX)
        delay_max = 0;

    return before_pc + delay_max > after_pc;
}

void Optimize(Program *program) {
    const int boot_address = 0x40000000;

    Program new_program;
    InitProgram(&new_program);
    static struct Operand* zero_reg;
    static struct Operand* one_imm;
    static struct Operand* goto_flag;
    static struct Instruction nop4;
    static struct Instruction nop6;

    zero_reg = ToOperand(OPERAND_REG, 0, 0, "$zero");
    one_imm = ToOperand(OPERAND_IMM, 0, 0, "1");
    goto_flag = ToOperand(OPERAND_REG, 0, 0, "$31");
    nop4 = (struct Instruction) {I_RI, "jmp", *zero_reg,  *zero_reg, *one_imm};
    nop6 = (struct Instruction) {I_RI, "goto",*goto_flag, *zero_reg, *one_imm};

    int cur_pc = - 1;
    int nop_len = 0;
    for (int i = 0; i < program->len; ++i) {
        InstructionPtr in = program->instruction_list[i];
        if (!in || in->type == I_INCLUDE)
            continue;
        int need_adjust = 1;
        while (need_adjust) {
                ++cur_pc;
                need_adjust = 0;
                int label_num = 0;
                for (int offset = 1; offset < 16 && cur_pc - offset >=0; ++offset) {
                        int prev_pc = cur_pc - offset;
                        InstructionPtr prev_in = (prev_pc < new_program.len) ? new_program.instruction_list[prev_pc] : NULL;
                        if (prev_in && prev_in->type == I_LABEL)
                                ++label_num;
                        if (prev_in && (!strcmp(prev_in->opname, "jmp") || !strcmp(prev_in->opname, "goto"))) {
                                break;
                        }
                        if (in && prev_in && Conflict(prev_in, prev_pc + label_num, in, cur_pc)) {
                                need_adjust = 1;
                                break;
                        }
                }
                if (!in || in->type == I_INCLUDE) {
                        continue;
                }
        }

        while (cur_pc - new_program.len - nop_len >= 1) {
                PushInstruction(&new_program, NULL);
        }

        // while (new_program.len + nop_len + 1 < cur_pc)
        //        PushInstruction(&new_program, NULL);
        PushInstruction(&new_program, in);
    }

    for (int i = 0; i + 1 < new_program.len; ++i)
        if (new_program.instruction_list[i+1] == NULL 
         && new_program.instruction_list[i]
         && new_program.instruction_list[i]->type == I_LABEL) {
                new_program.instruction_list[i+1] = new_program.instruction_list[i];
                new_program.instruction_list[i] = NULL;
        }

    free(program->instruction_list);
    *program = new_program;
}
