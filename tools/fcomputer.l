%option noyywrap

%{
#include <stdio.h>
int line;

void emit(const char* tag, const char* msg);
%}

NUMBER          ([0-9]+)
HEX             (0x[0-9a-fA-F]+)
ID              ([a-zA-Z_][a-zA-Z0-9_]*)
OPERATOR        ([-\+*%^|&(){},;=.])
STRING          (\042[^\042]*\042)

%%
{ID}            { emit("ID", yytext); }
{NUMBER}        { emit("Num", yytext); }
{HEX}           { emit("Hex", yytext); }
{STRING}        { emit("Str", yytext); }
{OPERATOR}      { emit("OP", yytext); }
"\n"            { ++line; }
.               { /* ignore this */ }
%%

int main(int argc, char* argv[]) {
        while (yylex()) {
                printf("%d: ", line);
        }
        return 0;
}



void emit(const char* tag, const char* msg) {
        printf("%s:\t\t%s\n", tag, msg);
}
