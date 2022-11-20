/**
 * Created by Tqcq Ann on 11/18/22.
**/
#ifndef FCOMPUTER_PARSER_OPCODE_H_
#define FCOMPUTER_PARSER_OPCODE_H_

#include <string.h>

struct OpCode {
    char name[10];
    int  opcode;
};

static struct OpCode opcode_table[] = {
    {"move", 26},
    {"li",   26},
    {"inc",  26},
    {"dec",  26},

    // rr
    {"and",  0},
    {"or",   1},
    {"xor",  2},
    {"shl",  3},
    {"sra",  4},
    {"srl",  5},

    {"add",  10},
    {"sub",  11},
    {"mul",  12},
    {"div",  13},
    {"mod",  14},
    {"pow",  15},

    // ri
    {"andi",  16},
    {"ori",   17},
    {"xori",  18},
    {"shli",  19},
    {"srai",  20},
    {"srli",  21},

    {"addi",  26},
    {"subi",  27},
    {"muli",  28},
    {"divi",  29},
    {"modi",  30},
    {"powi",  31},

    {"pop",   52},
    {"push",  53},
    {"lw",    54},
    {"sw",    55},

    {"rdrand",    56},
    {"rdtsc",     57},
    {"store_pc",  58},

    {"test",      60},
    {"lui",       61},

    {"goto",      62},
    {"jl",        62},
    {"je",        62},
    {"jg",        62},
    {"jle",       62},
    {"jge",       62},
    {"jne",       62},

    {"jmp",       63},
};

static int GetOpcode(const char* opname) {
    for (int i = 0; i < 64; ++i) {
        if (!strcmp(opname, opcode_table[i].name)) {
            return opcode_table[i].opcode;
        }
    }

    return -1;
}

#endif//FCOMPUTER_PARSER_OPCODE_H_
