#include "emit.h"
#include "instruction.h"
#include "program.h"
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *);

static InstructionPtr alloc_instruction(const char* opname, enum InstructionType type) {
    InstructionPtr in = (InstructionPtr)malloc(sizeof(struct Instruction));
    if (!in) {
        yyerror("alloc_instruction failed.");
    }
    memset(in, 0, sizeof(struct Instruction));
    strcpy(in->opname, opname);
    in->type = type;
    return in;
}

static char* unique_label(const char* prefix) {
    static int unique_id = 0;
    static char buf[256];
    strcpy(buf, prefix);
    sprintf(buf + strlen(buf), "%d", unique_id);
    ++unique_id;
    return buf;
}

void emit_include(const char *include_path) {
    InstructionPtr in = alloc_instruction("include", I_INCLUDE);
    strcpy(in->args[0].data, include_path);
    PushInstruction(GetProgram(), in);
}

void emit_label(const char *label) {
    InstructionPtr in = alloc_instruction("label", I_LABEL);
    strcpy(in->args[0].data, label);
    PushInstruction(GetProgram(), in);
}

void emit_rr(const char *opname, struct Operand *rd, struct Operand *rt, struct Operand *rs) {
    InstructionPtr in = alloc_instruction(opname, I_RR);
    in->args[0] = *rd;
    in->args[1] = *rt;
    in->args[2] = *rs;
    PushInstruction(GetProgram(), in);
}

void emit_ri(const char *opname, struct Operand *rd, struct Operand *rt, struct Operand *imm) {
    InstructionPtr in = alloc_instruction(opname, I_RI);
    in->args[0] = *rd;
    in->args[1] = *rt;
    in->args[2] = *imm;
    PushInstruction(GetProgram(), in);
}

void emit_stack(const char* opname, struct Operand *rd) {
    struct Operand* sp =  ToOperand(OPERAND_REG, 0, 0, "$sp");
    struct Operand* imm_pop = ToOperand(OPERAND_IMM, 0, 0, "0");
    struct Operand* imm_push = ToOperand(OPERAND_IMM, 0, 0, "-1");

    if (!strcmp(opname, "push")) {
        emit_ri(opname, rd, sp, imm_push);
    } else if (!strcmp(opname, "pop")) {
        emit_ri(opname, rd, sp, imm_pop);
    }

    free(sp);
    free(imm_pop);
    free(imm_push);
}

void emit_loadsave(const char *opname, struct Operand *rd, struct Operand *offset, struct Operand *base) {
    emit_ri(opname, rd, base, offset);
}

void emit_bin(const char *opname, struct Operand *dst, struct Operand *src) {
    struct Operand *zero_imm = ToOperand(OPERAND_IMM, 0, 0, "0");
    struct Operand *zero_reg = ToOperand(OPERAND_REG, 0, 0, "$zero");
    if (src->type == OPERAND_IMM) {
        emit_ri(opname, dst, zero_reg, src);
    } else {
        emit_ri(opname, dst, src, zero_imm);
    }
    free(zero_imm);
    free(zero_reg);
}

void emit_loadimm(const char *opname, struct Operand *dst, struct Operand *imm) {
    struct Operand *zero_reg = ToOperand(OPERAND_REG, 0, 0, "$0");

    struct Operand *imm_hi = ToOperand(imm->type, 1, 0, imm->data);
    struct Operand *imm_lo = ToOperand(imm->type, 0, 1, imm->data);
    if (!strcmp("li", opname)) { // li reg,imm
        int HasHi = (OperandToValue(imm) & 0xFFFF0000) || imm->type != OPERAND_IMM;
        int HasLo = (OperandToValue(imm) & 0x0000FFFF) || imm->type != OPERAND_IMM;
        if (HasHi) {
            emit_loadimm("lui", dst, imm_hi);
            if (HasLo) { // need merge
                emit_ri("ori", dst, dst, imm_lo);
            }
        } else if (HasLo){ // only lower
            emit_ri("addi", dst, zero_reg, imm_lo);
        } else { // reset zero
            emit_bin("move", dst, imm);
        }
    } else if (!strcmp("lui", opname)){ // lui reg,imm
        emit_ri(opname, dst, zero_reg, imm);
    } else {
        yyerror("emit_loadimm failed.");
    }
    free(zero_reg);
    free(imm_hi);
    free(imm_lo);
}

void emit_branch(const char *opname, struct Operand *offset, struct Operand *base) {
    struct Operand *jl_flag = ToOperand(OPERAND_REG, 0, 0,   "$12");   // 01100
    struct Operand *je_flag = ToOperand(OPERAND_REG, 0, 0,   "$10");   // 01010
    struct Operand *jg_flag = ToOperand(OPERAND_REG, 0, 0,   "$9");    // 01001
    struct Operand *jle_flag = ToOperand(OPERAND_REG, 0, 0,  "$14");   // 01110
    struct Operand *jge_flag = ToOperand(OPERAND_REG, 0, 0,  "$11");   // 01011
    struct Operand *jne_flag = ToOperand(OPERAND_REG, 0, 0,  "$13");   // 01101
    struct Operand *goto_flag = ToOperand(OPERAND_REG, 0, 0, "$23");   // 10111

    if (!strcmp(opname, "goto")) {
        emit_ri(opname, goto_flag, base, offset);
    } else if (!strcmp(opname, "jl")) {
        emit_ri(opname, jl_flag, base, offset);
    } else if (!strcmp(opname, "je")) {
        emit_ri(opname, je_flag, base, offset);
    } else if (!strcmp(opname, "jg")) {
        emit_ri(opname, jg_flag, base, offset);
    } else if (!strcmp(opname, "jle")) {
        emit_ri(opname, jle_flag, base, offset);
    } else if (!strcmp(opname, "jge")) {
        emit_ri(opname, jge_flag, base, offset);
    } else if (!strcmp(opname, "jne")) {
        emit_ri(opname, jne_flag, base, offset);
    }

    free(jl_flag);
    free(je_flag);
    free(jg_flag);
    free(jle_flag);
    free(jge_flag);
    free(jne_flag);
    free(goto_flag);
}

void emit_read(const char* opname, struct Operand* dst) {
    struct Operand *zero_reg = ToOperand(OPERAND_REG, 0, 0, "$0");
    struct Operand *zero_imm = ToOperand(OPERAND_IMM, 0, 0, "0");

    emit_ri(opname, dst, zero_reg, zero_imm);

    free(zero_reg);
    free(zero_imm);
}


void emit_jmp(const char *opname, struct Operand *address) {
    struct Operand *zero = ToOperand(OPERAND_IMM, 0, 0, "0");

    emit_ri(opname, zero, zero, address);

    free(zero);
}

void emit_call(const char *opname, struct Operand *offset, struct Operand *base) {
    const char* label = unique_label("__system_unique_ret_label_");
    struct Operand *ret_label = ToOperand(OPERAND_LABEL, 0, 0, label);

    struct Operand *zero = ToOperand(OPERAND_REG, 0, 0, "$zero");
    struct Operand *ra= ToOperand(OPERAND_REG, 0, 0, "$ra");
    struct Operand *goto_flag = ToOperand(OPERAND_REG, 0, 0, "$23"); // 10111

    emit_ri("store_pc", ra, zero, ret_label);
    emit_stack("push", ra);
    if (OperandToValue(base) == 0 && offset->type == OPERAND_LABEL) {
        emit_jmp("jmp", offset);
    } else {
        emit_branch("goto", offset, base);
    }
    emit_label(label);

    free(ret_label);
    free(zero);
    free(ra);
    free(goto_flag);
}

void emit_ret(const char *opname) {
    struct Operand *zero = ToOperand(OPERAND_IMM, 0, 0, "0");
    // struct Operand *inv= ToOperand(OPERAND_IMM, 0, 0, "-1");
    struct Operand *at= ToOperand(OPERAND_REG, 0, 0, "$ra");

    emit_stack("pop", at);
    emit_branch("goto", zero, at);

    free(zero);
    free(at);
}

void emit_jal(const char* opname, struct Operand* jmp_label) {
    const char* jr_label = unique_label("__system_unique_jr_label_");
    struct Operand *ret_label = ToOperand(OPERAND_LABEL, 0, 0, jr_label);

    struct Operand *zero = ToOperand(OPERAND_REG, 0, 0, "$zero");
    struct Operand *ra= ToOperand(OPERAND_REG, 0, 0, "$ra");

    emit_ri("store_pc", ra, zero, ret_label);
    emit_jmp("jmp", jmp_label);
    emit_label(jr_label);

    free(ret_label);
    free(zero);
    free(ra);
}
void emit_jr(const char* opname, struct Operand* ret_reg) {
    struct Operand *zero = ToOperand(OPERAND_IMM, 0, 0, "0");
    struct Operand *at= ToOperand(OPERAND_REG, 0, 0, "$ra");

    emit_branch("goto", zero, at);

    free(zero);
    free(at);
}

void emit_inc_dec(const char *opname, struct Operand *dst) {
    struct Operand* one = ToOperand(OPERAND_IMM, 0, 0, "1");
    struct Operand* inv = ToOperand(OPERAND_IMM, 0, 0, "-1");

    if (!strcmp(opname, "inc")) {
        emit_ri(opname, dst, dst, one);
    } else if (!strcmp(opname, "dec")) {
        emit_ri(opname, dst, dst, inv);
    }


    free(one);
    free(inv);
}
