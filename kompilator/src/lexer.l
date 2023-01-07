%{
    #include <iostream>
    #include <string>
    #include <fstream>
    #include "parser.hpp"

    int yylex();

    using namespace std;
%}


%option noyywrap


%x COMMENT


IDENTIF         [_a-z]+
NUMBER          [0-9]+
SPACE           " "


%%

"["             { ECHO; BEGIN(COMMENT); }
<COMMENT>[^]]   ECHO;
<COMMENT>"]"    { ECHO; BEGIN(0); }


\n              ECHO;
\t              ECHO;
{SPACE}         ECHO;


"PROCEDURE"     { ECHO; return T_PROCEDURE; }
"PROGRAM"       { ECHO; return T_PROGRAM; }
"IS"            { ECHO; return T_IS; }
"VAR"           { ECHO; return T_VAR; }
"BEGIN"         { ECHO; return T_BEGIN; }
"END"           { ECHO; return T_END; }
"IF"            { ECHO; return T_IF; }
"THEN"          { ECHO; return T_THEN; }
"ELSE"          { ECHO; return T_ELSE; }
"ENDIF"         { ECHO; return T_ENDIF; }
"WHILE"         { ECHO; return T_WHILE; }
"DO"            { ECHO; return T_DO; }
"ENDWHILE"      { ECHO; return T_ENDWHILE; }
"REPEAT"        { ECHO; return T_REPEAT; }
"UNTIL"         { ECHO; return T_UNTIL; }
"READ"          { ECHO; return T_READ; }
"WRITE"         { ECHO; return T_WRITE; }


{IDENTIF}       { ECHO; return T_IDENTIFIER; }
{NUMBER}        { yylval = atoll(yytext); cout << yylval; return T_NUM; }


":="            { ECHO; return T_ASSIGN; }
","             { ECHO; return T_COMMA; }
";"             { ECHO; return T_SEMICOLON; }
":"             { ECHO; return T_COLON; }

"="             { ECHO; return T_EQ; }
"!="            { ECHO; return T_NE; }
">"             { ECHO; return T_GT; }
"<"             { ECHO; return T_LT; }
">="            { ECHO; return T_GE; }
"<="            { ECHO; return T_LE; }

"+"             { ECHO; return T_ADD; }
"-"             { ECHO; return T_SUB; }
"*"             { ECHO; return T_MUL; }
"/"             { ECHO; return T_DIV; }
"%"             { ECHO; return T_MOD; }
"("             { ECHO; return T_LPAR; }
")"             { ECHO; return T_RPAR; }


.               cout << "Unknown token";

%%


void run_lexer(FILE *file)
{
    cout << "Starting lexing...\n\n";

    yyin = file;

    while(yylex());

    cout << "\n";
}