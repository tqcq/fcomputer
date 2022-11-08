/**
 * Created by Tqcq Ann on 11/8/22.
**/

#include <iostream>
#include <string>
#include <vector>

class ASMInstruction {
 public:
  virtual ~ASMInstruction() {};
  virtual std::string name() = 0;
  virtual int opcode() = 0;
  virtual int raw_tick() = 0;
  virtual int war_tick() = 0;
  virtual int waw_tick() = 0;
  virtual std::vector<std::string> keys() = 0;
};

#define DECL_ASM(name_, opcode_, raw_tick_, war_tick_, waw_tick_, keys_...) \
  class name_##ASMInstruction : public ASMInstruction {                    \
  public:                                                    \
    name_##ASMInstruction(std::vector<std::string> args) {}          \
    ~name_##ASMInstruction() override {}                             \
    std::string name() override { return #name_; }                     \
    int opcode() override { return opcode_; }                          \
    int raw_tick() override { return raw_tick_; } \
    int waw_tick() override { return waw_tick_; } \
    int war_tick() override { return war_tick_; }           \
    std::vector<std::string> keys() override { std::vector<std::string> result = { #name_, ##keys_ }; return result;  }        \
  }

DECL_ASM(ADDU,  1, 5, 5, 5);
DECL_ASM(SUBU,  2, 5, 5, 5);
DECL_ASM(MUL,   3, 5, 5, 5);
DECL_ASM(DIV,   4, 5, 5, 5);
DECL_ASM(MOD,   5, 5, 5, 5);
DECL_ASM(AND,   6, 5, 5, 5);
DECL_ASM(OR,    7, 5, 5, 5);
DECL_ASM(XOR,   8, 5, 5, 5);
DECL_ASM(POW,   9, 5, 5, 5);
DECL_ASM(SLAV,  10, 5, 5, 5);
DECL_ASM(SRAV,  11, 5, 5, 5);
DECL_ASM(SRLV,  12, 5, 5, 5);
DECL_ASM(SLA,   13, 5, 5, 5);
DECL_ASM(SRA,   14, 5, 5, 5);
DECL_ASM(SRL,   15, 5, 5, 5);

DECL_ASM(ADDUI, 33, 5, 5, 5);
DECL_ASM(SUBUI, 34, 5, 5, 5);
DECL_ASM(MULI,  35, 5, 5, 5);
DECL_ASM(DIVI,  36, 5, 5, 5);
DECL_ASM(MODI,  37, 5, 5, 5);
DECL_ASM(ANDI,  38, 5, 5, 5);
DECL_ASM(ORI,   39, 5, 5, 5);
DECL_ASM(XORI,  40, 5, 5, 5);
DECL_ASM(POWI,  41, 5, 5, 5);


DECL_ASM(LW,    48, 5, 5, 5);
DECL_ASM(SW,    49, 5, 5, 5);

DECL_ASM(POP,   52, 5, 5, 5);
DECL_ASM(PUSH,  53, 5, 5, 5);

DECL_ASM(B,     54, 5, 5, 5, "jr", "jl", "je", "jg", "jle", "jge", "jne");
DECL_ASM(LUI,   55, 5, 5, 5);

DECL_ASM(J,     63, 5, 5, 5, "j");

int main(int argc, char *argv[]) {
  return 0;
}