/**
 * Created by Tqcq Ann on 11/18/22.
**/

#include "program.h"

static Program program;

void InitProgram(Program* new_program) {
    if (!new_program) {
        return;
    }

    new_program->instruction_list = (InstructionPtr*) malloc(MAX_PROGRAM_SIZE * sizeof(InstructionPtr));
    if (!new_program->instruction_list) {
        yyerror("alloc instruction list failed");
    }
    new_program->len = 0;
}

void PushInstruction(Program* program, InstructionPtr in) {
    if (!program->instruction_list) {
        InitProgram(program);
    }

    program->instruction_list[program->len] = in;
    program->len++;
}

void DisplayProgram(Program *program) {
    if (!program)
        return;

    for (int i = 0; i < program->len; i++) {
        const char* buf = InstructionToString(program->instruction_list[i]);
        puts(buf);
    }
}

Program* GetProgram() {
    return &program;
}