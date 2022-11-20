/**
 * Created by Tqcq Ann on 11/17/22.
**/
#ifndef FCOMPUTER_PARSER_INSTRUCTION_H_
#define FCOMPUTER_PARSER_INSTRUCTION_H_

#include "opcode.h"
#include "operand.h"
#include <stdio.h>
#include <stdlib.h>

enum InstructionType {
    I_INCLUDE,
    I_LABEL,
    I_RR,
    I_RI,
};

struct Instruction {
    enum InstructionType type;

    char opname[10];
    // rd, rt, (rs or imm or shmat)
    struct Operand args[3];
};
typedef struct Instruction *InstructionPtr;

static const char *InstructionToString(InstructionPtr instruction) {
    static char buf[256];
    if (instruction == NULL)
        return "nop";

    if (instruction->type == I_LABEL) {
        sprintf(buf, "%s: ", instruction->args[0].data);
    }

    if (instruction->type == I_INCLUDE) {
        sprintf(buf, "#include %s", instruction->args[0].data);
    }

    if (instruction->type == I_RI || instruction->type == I_RR) {
        int opcode = GetOpcode(instruction->opname);
        char value[256];
        if (instruction->args[2].type == OPERAND_LABEL || instruction->args[2].type == OPERAND_REG) {
            sprintf(value, "%s", instruction->args[2].data);
        } else  {
            sprintf(value, "%d", OperandToValue(instruction->args + 2));
        }

        sprintf(buf, "%s %s %s %s", instruction->opname,
                instruction->args[0].data,
                instruction->args[1].data,
                value);
    }

    return buf;
}

static int GenCode(InstructionPtr instruction) {
    if (instruction == NULL)
        return 0;

    int opcode = GetOpcode(instruction->opname);
    int x = 0;

    int args[] = {
        OperandToValue(instruction->args),
        OperandToValue(instruction->args + 1),
        OperandToValue(instruction->args + 2),
    };

    x = opcode | (args[0] << 6) | (args[1] << 11) | (args[2] << 16);

    return x;
}

#endif// FCOMPUTER_PARSER_INSTRUCTION_H_
