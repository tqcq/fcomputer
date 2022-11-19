%option noyywrap
%{
#include <stdio.h>
#include "y.tab.h"
void yyerror(const char*);
void emit(const char* msg, const char* data);
%}

COMMENT1 ;[^\n\r]*

WS       [ \t\b]+
NEWLINE  [\n\r]
REGISTER \$(zero|at|gp|sp|fp|ra|[kv][0-1]|a[0-3]|t[0-9]|s[0-7]|[0-9]+)
HEX      0x[a-fA-F0-9]+
NUMBER   [0-9]+
LABEL    [a-zA-Z_][a-zA-Z_0-9]*
OPERATOR [(),:]
STRING   \"[^\n\"]*\"

HILO     (%hi)|(%lo)
INCLUDE  (#include)

RR_I     ("and"|"or"|"xor"|"shl"|"sra"|"srl"|"add"|"sub"|"mul"|"div"|"mod"|"pow")
RI_I     {RR_I}i
STACK_I  ("push"|"pop")
LS_I     ("lw"|"sw")
BIN_I    ("test"|"move")
LOAD_IMM_I ("lui"|"li")
BRANCH_I ("jl"|"je"|"jg"|"jle"|"jge"|"jne")
JMP_I    ("jmp"|"j")
CALL_I   ("call")
RET_I    ("ret")
INC_DEC_I  ("inc"|"dec")

%%
{COMMENT1}  /* empty comment */
{WS}        /* empty comment */
{NEWLINE}   /* empty comment */

{STRING}    { emit("String  \t", yytext); yylval.sData = strdup(yytext); return T_STRING; }
{INCLUDE}   { emit("Include \t", yytext); return T_INCLUDE; }
{HILO}      { emit("HI-LO   \t", yytext); yylval.sData = strdup(yytext); return T_HILO; }
{REGISTER}  { emit("Register\t", yytext); yylval.sData = strdup(yytext); return T_REGISTER; }
{OPERATOR}  { emit("Operator\t", yytext); return *yytext; }
{RR_I}      { emit("RR_I    \t", yytext); yylval.sData = strdup(yytext); return T_RR_I; }
{RI_I}      { emit("RI_I    \t", yytext); yylval.sData = strdup(yytext); return T_RI_I; }
{STACK_I}   { emit("STACK_I \t", yytext); yylval.sData = strdup(yytext); return T_STACK_I; }
{LS_I}      { emit("LS_I    \t", yytext); yylval.sData = strdup(yytext); return T_LS_I; }
{BIN_I}     { emit("BIN_I   \t", yytext); yylval.sData = strdup(yytext); return T_BIN_I; }
{LOAD_IMM_I} { emit("LOAD_IMM_I   \t",yytext);  yylval.sData = strdup(yytext); return T_LOAD_IMM_I; }
{BRANCH_I}  { emit("BRANCH_I\t", yytext); yylval.sData = strdup(yytext); return T_BRANCH_I; }
{JMP_I}     { emit("JMP_I   \t", yytext); yylval.sData = strdup(yytext); return T_JMP_I; }
{CALL_I}    { emit("CALL_I  \t", yytext); yylval.sData = strdup(yytext); return T_CALL_I; }
{RET_I}     { emit("RET_I   \t", yytext); yylval.sData = strdup(yytext); return T_RET_I; }
{INC_DEC_I} { emit("INC_DEC \t", yytext); yylval.sData = strdup(yytext); return T_INC_DEC_I; }
{LABEL}     { emit("Label   \t", yytext); yylval.sData = strdup(yytext); return T_LABEL; }

{HEX}       { emit("HEX     \t", yytext); yylval.sData = strdup(yytext); return T_HEX; }
{NUMBER}    { emit("NUMBER  \t", yytext); yylval.sData = strdup(yytext); return T_NUMBER; }
.           { emit("Error   \t", yytext); }
%%

/*
int main(int argc, char* argv[]) {
    while (yylex());
    return 0;
}
*/

void emit(const char* msg, const char* data) {
    // printf("%s:\t%s\n", msg, data);
}
