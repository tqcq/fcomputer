lexer grammar FComputerASM;

COMMENT: ';' .+? -> skip;
WS: [ \t\r\n\b]+ -> skip;
