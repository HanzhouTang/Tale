grammar Expr;

prog            :   stat*;

stat            :   expr NEWLINE                                 //need change to support assign
                |   NEWLINE
                ;


expr            :   '-' expr                                                        #reverse
                |   expr '=' expr                                                   #assign
                |   'lambda' ID? '(' exprList? ')' NEWLINE? '{' expr* '}'           #lambda
                |   '{' keyList '}'                                                 #dictionary
                |  '{' expr* '}'                                                    #closure
                |   expr op=('*'|'/') expr                                          #MulDiv
                |   expr op=('+'|'-') expr                                          #AddSub
                |   expr '(' exprList? ')'                                          #call
                |   '(' expr ')'                                                    #parens
                |   'null'                                                          #null
                |   ID                                                              #id
                |   STRING                                                          #string
                |   NUMBER                                                          #number
                ;

exprList        : expr (',' expr)*;
keyList         : STRING ':' expr (','STRING ':' expr )*;
//====================================================== TOKENS =========================================================


ID              :   [a-zA-Z_] ([a-zA-Z_] | [0-9])*;

fragment
INT             :   '0' | [1-9] [0-9]* ;

NUMBER          :  INT '.' [0-9]+
                |  INT
                ;
fragment
ESC             :   '\\' ["\\/bfnrt];

STRING          :   '"' (ESC | ~["\\])* '"' ;

WS              :   [ \t] ->skip;

NEWLINE         :   '\r'?   '\n';

COMMENT         :   '#' .*? NEWLINE ->skip;

MUL             :   '*';

DIV             :   '/';

ADD             :   '+';

SUB             :   '-';