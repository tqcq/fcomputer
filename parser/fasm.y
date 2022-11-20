%{
#include <stdio.h>
#include "operand.h"
#include "emit.h"
#include "program.h"
#include "optimize.h"
#include "pass/pass_update_label.h"
#include "pass/pass_reduce_program.h" 
int yylex(void);
void yyerror(const char *);
extern Program program;
%}

%union {
    int iValue;
    char *sData;
    struct Operand* ptrOperand;
}

%token <iValue> T_INCLUDE
%token <sData>  T_STRING

%token <sData>  T_REGISTER
%token <sData>  T_HEX
%token <sData>  T_NUMBER
%token <sData>  T_LABEL
%token <iValue> T_OPERATOR

%token <sData>  T_HILO

%token <sData> T_RR_I
%token <sData> T_RI_I
%token <sData> T_STACK_I
%token <sData> T_LS_I
%token <sData> T_BIN_I
%token <sData> T_LOAD_IMM_I
%token <sData> T_BRANCH_I
%token <sData> T_READ_I
%token <sData> T_JMP_I
%token <sData> T_CALL_I
%token <sData> T_RET_I
%token <sData> T_INC_DEC_I

%type  <ptrOperand> imm register

%%
program
    : program stmt
    |
    ;

stmt
    : INCLUDE_STMT
    | LABEL_DEF_STMT
    | RR_I_STMT
    | RI_I_STMT
    | STACK_I_STMT
    | LS_I_STMT
    | BIN_I_STMT
    | LOAD_IMM_I_STMT
    | BRANCH_I_STMT
    | READ_I_STMT
    | JMP_I_STMT
    | CALL_I_STMT
    | RET_I_STMT
    | INC_DEC_I_STMT
    ;

register
    : T_REGISTER                { $$=ToOperand(OPERAND_REG,   0, 0, $1); }
    ;

imm
    : T_HEX                     { $$=ToOperand(OPERAND_IMM,   0, 0, $1); }
    | T_NUMBER                  { $$=ToOperand(OPERAND_IMM,   0, 0, $1); }
    | T_LABEL                   { $$=ToOperand(OPERAND_LABEL, 0, 0, $1); }
    | T_HILO '(' T_HEX ')'      { $$=ToOperand(OPERAND_IMM,   IS_HI($1), IS_LO($1), $3); }
    | T_HILO '(' T_NUMBER ')'   { $$=ToOperand(OPERAND_IMM,   IS_HI($1), IS_LO($1), $3); }
    | T_HILO '(' T_LABEL ')'    { $$=ToOperand(OPERAND_LABEL, IS_HI($1), IS_LO($1), $3); }
    ;

INCLUDE_STMT:    T_INCLUDE  T_STRING                                 { emit_include($2);  };
LABEL_DEF_STMT:  T_LABEL    ':'                                      { emit_label($1); };
RR_I_STMT:       T_RR_I     register ',' register ',' register       { emit_rr($1, $2, $4, $6); };
RI_I_STMT:       T_RI_I     register ',' register ',' imm            { emit_ri($1, $2, $4, $6); };
STACK_I_STMT:    T_STACK_I  register                                 { emit_stack($1, $2); };
LS_I_STMT:       T_LS_I     register ',' imm '(' register ')'        { emit_loadsave($1, $2, $4, $6); };
BIN_I_STMT
    :      T_BIN_I    register ',' imm                               { emit_bin($1, $2, $4); }
    |      T_BIN_I    register ',' register                          { emit_bin($1, $2, $4); }
    ;
LOAD_IMM_I_STMT: T_LOAD_IMM_I    register ',' imm                    { emit_loadimm($1, $2, $4); };
BRANCH_I_STMT
    :   T_BRANCH_I imm '(' register ')'                              { emit_branch($1, $2, $4); }
    |   T_BRANCH_I imm                                               { emit_branch($1, $2, ToOperand(OPERAND_REG, 0, 0, "$0")); }
    ;
READ_I_STMT:     T_READ_I   register                                 { emit_read($1, $2) };
JMP_I_STMT:      T_JMP_I    imm                                      { emit_jmp($1, $2); };
CALL_I_STMT
    :   T_CALL_I   imm '(' register ')'                              { emit_call($1, $2, $4); }
    |   T_CALL_I   imm                                               { emit_call($1, $2, ToOperand(OPERAND_REG,   0, 0, "$0")); }
    ;
RET_I_STMT:      T_RET_I                                             { emit_ret($1); };
INC_DEC_I_STMT:  T_INC_DEC_I register                                { emit_inc_dec($1, $2); };

%%

void yyerror(const char* msg) {
    fprintf(stderr,"%s\n", msg);
    return;
}

void PreAction() {
    struct Operand *stack_size = ToOperand(OPERAND_IMM, 0, 0, "2048");
    struct Operand *sp_reg= ToOperand(OPERAND_REG, 0, 0, "$sp");
    emit_bin("move", sp_reg, stack_size);
}

void AfterAction() {
    struct Operand *end_label= ToOperand(OPERAND_LABEL, 0, 0, "__system_end_loop");
    emit_label("__system_end_loop");
    emit_jmp("jmp", end_label);
}

int main(int argc, char* argv[]) {
    const int program_size = 10000;

    PreAction();
    yyparse();
    AfterAction();

    Optimize(GetProgram());
    int nop_instruction = 0;
    int all_instruction = GetProgram()->len;
    for (int i = 0; i < all_instruction; ++i)
        if (GetProgram()->instruction_list[i] == NULL)  {
            ++nop_instruction;
        } else if (GetProgram()->instruction_list[i]->type == I_LABEL || GetProgram()->instruction_list[i]->type == I_INCLUDE) {
            --all_instruction;
        }
    float percent = 1.0 * (all_instruction-nop_instruction) / all_instruction;

    int offset = 0;
    if (argc > 1 && argv[1][0] == 'p') {
            printf(">>> %d/%d -> %d Hz, cpu-time: %f <<<\n", all_instruction - nop_instruction, all_instruction, (int)(60 * percent), percent);
            for (int i = 0; i < GetProgram()->len; ++i) {
                    int code = GenCode(GetProgram()->instruction_list[i]);

                    printf("%20d %3d: ", code, i + offset);
                    InstructionPtr in = GetProgram()->instruction_list[i];
                    if (in && in->type == I_LABEL) {
                        --offset;
                    } else {
                        printf("    ");
                    }
                    puts(InstructionToString(GetProgram()->instruction_list[i]));
            }
    }

    PassReduceProgram(GetProgram());
    PassUpdateLabel(GetProgram());

    if (argc > 1) {
        if (argv[1][0] == 'x') {
            printf(">>> %d/%d -> %d Hz, cpu-time: %f <<<\n", all_instruction - nop_instruction, all_instruction, (int)(60 * percent), percent);
            for (int i = 0; i < GetProgram()->len; ++i) {
                    int code = GenCode(GetProgram()->instruction_list[i]);

                    printf("%20d %3d: ", code, i + offset);
                    InstructionPtr in = GetProgram()->instruction_list[i];
                    if (in && in->type == I_LABEL) {
                        --offset;
                    } else {
                        printf("    ");
                    }
                    puts(InstructionToString(GetProgram()->instruction_list[i]));
            }
        }
    } else {
        int offset = 0;
        for (int i = 0; i < GetProgram()->len; ++i) {
                InstructionPtr in = GetProgram()->instruction_list[i];
                if (in && in->type == I_LABEL) {
                    --offset;
                    continue;
                }
                int code = GenCode(GetProgram()->instruction_list[i]);
                if (i > 0) {
                    puts(",");
                }

                printf("0x%x, 0x%x, 0x%x, 0x%x",
                                        (code >> 24) & 0xFF,
                                        (code >> 16) & 0xFF,
                                        (code >> 8)  & 0xFF,
                                        code & 0xFF);
        }
        puts("");
    }
    return 0;
}
