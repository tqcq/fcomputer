/**
 * Created by Tqcq Ann on 11/17/22.
**/
#ifndef FCOMPUTER_PARSER_OPERAND_H_
#define FCOMPUTER_PARSER_OPERAND_H_

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define IS_HI(str) (strcmp(str, "%hi") == 0)
#define IS_LO(str) (strcmp(str, "%lo") == 0)

extern void yyerror(const char*);

enum OperandType {
    OPERAND_IMM,
    OPERAND_REG,
    OPERAND_LABEL,
};

struct Operand {
    enum OperandType type;
    int is_hi;
    int is_lo;

    char data[100];
};

static struct Operand* ToOperand(enum OperandType type, int is_hi, int is_lo, const char* data) {
    struct Operand* ptr;
    if ((ptr = (struct Operand*)malloc(sizeof(struct Operand))) == NULL)
        yyerror("ToOperand Failed");

    strcpy(ptr->data, data);
    ptr->type = type;
    ptr->is_hi = is_hi;
    ptr->is_lo = is_lo;
    return ptr;
}

static int OperandToValue(struct Operand* operand) {
    struct RegIdx {
        const char name[10];
        int idx;
    };

    static struct RegIdx table[] = {
        {"zero", 0},
        {"at",   1},

        {"v0",   2},
        {"v1",   3},

        {"a0",   4},
        {"a1",   5},
        {"a2",   6},
        {"a3",   7},

        {"t0",   8},
        {"t1",   9},
        {"t2",   10},
        {"t3",   11},
        {"t4",   12},
        {"t5",   13},
        {"t6",   14},
        {"t7",   15},

        {"s0",   16},
        {"s1",   17},
        {"s2",   18},
        {"s3",   19},
        {"s4",   20},
        {"s5",   21},
        {"s6",   22},
        {"s7",   23},

        {"t8",   24},
        {"t9",   25},

        {"gp",   28},
        {"sp",   29},
        {"fp",   30},
        {"ra",   31},
    };

    if (operand->type == OPERAND_REG) {
        if (isdigit(operand->data[1])) {
            return strtol(operand->data + 1, NULL, 10);
        }

        int reg_idx = -1;
        for (int i = 0; i < sizeof(table); ++i) {
            if (!strcmp(operand->data+1, table[i].name)) {
                reg_idx = table[i].idx;
                break;
            }
        }
        if (reg_idx == -1) {
            yyerror("invalid register");
        }

        return reg_idx;
    } else if (operand->type == OPERAND_IMM) {
        int value = 0;
        if (strstr(operand->data, "x") != NULL) {
            // hex
            value = strtol(operand->data, NULL, 16);
        } else {
            // decimal
            value = strtol(operand->data, NULL, 10);
        }

        if (operand->is_hi)
            value = (value >> 16) & 0xFFFF;
        if (operand->is_lo)
            value = value & 0xFFFF;

        return value;
    } else if (operand->type == OPERAND_LABEL) {
        return 1314;
    }

    return 0;
}

#endif // FCOMPUTER_PARSER_OPERAND_H_
