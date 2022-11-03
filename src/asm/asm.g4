grammar asm;

ID: [a-zA-Z][a-zA-Z0-9_]*;
OP: [a-zA-Z]+;
ANY_SPACE: [ \t\r\n]+;
REG: '$' [0-7];
NUM: [0-9]+;
HEX: '0x' [0-9A-Za-z]+;
COMMENT: ';' .*?;

// ADD, SUB, MUL, DIV, MOD, POW, SHL, SHR, SND, OR, XOR
// A(REG1) OP B(REG2) = Result(REG3)
RExpr: OP ',' REG ',' REG ',' REG;

// LW, SW, BX
IExpr
    : OP ',' REG ',' NUM(REG)
    | OP ',' REG ',' REG
    | OP ',' REG ',' HEX(REG)
    ;

// JUMP
JExpr
    : OP ',' NUM
    | OP ',' HEX
    ;

WS: [ \t\r\n]+ -> skip;
