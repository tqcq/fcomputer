/**
 * Created by Tqcq Ann on 11/18/22.
**/
#ifndef FCOMPUTER_PARSER_PROGRAM_H_
#define FCOMPUTER_PARSER_PROGRAM_H_
#include "instruction.h"

extern void yyerror(const char*);

enum {
    MAX_PROGRAM_SIZE = 10240
};

typedef struct {
    InstructionPtr *instruction_list;
    int len;
} Program;


void InitProgram(Program* new_program);
void PushInstruction(Program* program, InstructionPtr in);
Program* GetProgram();

#endif//FCOMPUTER_PARSER_PROGRAM_H_
