/**
 * Created by Tqcq Ann on 11/17/22.
**/
#ifndef FCOMPUTER_PARSER_EMIT_H_
#define FCOMPUTER_PARSER_EMIT_H_
#include "operand.h"

void emit_include(const char* include_path);
void emit_label(const char* label);
void emit_rr(const char* opname, struct Operand* rd, struct Operand* rt, struct Operand* rs);
void emit_ri(const char* opname, struct Operand* rd, struct Operand* rt, struct Operand* imm);
void emit_stack(const char* opname, struct Operand* rd);
void emit_loadsave(const char* opname, struct Operand* rd, struct Operand* offset, struct Operand* base);
void emit_bin(const char* opname, struct Operand* dst, struct Operand* src);
void emit_loadimm(const char* opname, struct Operand* dst, struct Operand* imm);
void emit_branch(const char* opname, struct Operand* offset, struct Operand* base);
void emit_read(const char* opname, struct Operand* dst);
void emit_jmp(const char* opname, struct Operand* address);
void emit_call(const char* opname, struct Operand* offset, struct Operand* base);
void emit_ret(const char* opname);
void emit_jal(const char* opname, struct Operand* jmp_label);
void emit_jr(const char* opname, struct Operand* ret_reg);
void emit_inc_dec(const char* opname, struct Operand* dst);

#endif // FCOMPUTER_PARSER_EMIT_H_
