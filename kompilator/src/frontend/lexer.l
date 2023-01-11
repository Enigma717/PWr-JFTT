    /*
        Autor:      Marek Traczyński (261748)
        Kurs:       Języki Formalne i Techniki Translacji
        Projekt:    Kompilator
        Opis pliku: Lexer
    */


%{
    #include <iostream>
    #include <string>
    #include <fstream>
    #include "./parser.hpp"

    using std::cout;

    int yylex();
%}


%option noyywrap


%x COMMENT


IDENTIFIER      [_a-z]+
NUMBER          [0-9]+
SPACE           " "


%%

"["             { ECHO; BEGIN(COMMENT); }
<COMMENT>[^]]   ECHO;
<COMMENT>"]"    { ECHO; BEGIN(0); }


[ \t\n]         ECHO;


"PROCEDURE"     { ECHO; return yylval.token = T_PROCEDURE; }
"PROGRAM"       { ECHO; return yylval.token = T_PROGRAM; }
"IS"            { ECHO; return yylval.token = T_IS; }
"VAR"           { ECHO; return yylval.token = T_VAR; }
"BEGIN"         { ECHO; return yylval.token = T_BEGIN; }
"END"           { ECHO; return yylval.token = T_END; }
"IF"            { ECHO; return yylval.token = T_IF; }
"THEN"          { ECHO; return yylval.token = T_THEN; }
"ELSE"          { ECHO; return yylval.token = T_ELSE; }
"ENDIF"         { ECHO; return yylval.token = T_ENDIF; }
"WHILE"         { ECHO; return yylval.token = T_WHILE; }
"DO"            { ECHO; return yylval.token = T_DO; }
"ENDWHILE"      { ECHO; return yylval.token = T_ENDWHILE; }
"REPEAT"        { ECHO; return yylval.token = T_REPEAT; }
"UNTIL"         { ECHO; return yylval.token = T_UNTIL; }
"READ"          { ECHO; return yylval.token = T_READ; }
"WRITE"         { ECHO; return yylval.token = T_WRITE; }


"("             { ECHO; return T_LPAR; }
")"             { ECHO; return T_RPAR; }
":="            { ECHO; return T_ASSIGN; }
","             { ECHO; return T_COMMA; }
";"             { ECHO; return T_SEMICOLON; }

"+"             { ECHO; return T_ADD; }
"-"             { ECHO; return T_SUB; }
"*"             { ECHO; return T_MUL; }
"/"             { ECHO; return T_DIV; }
"%"             { ECHO; return T_MOD; }

"="             { ECHO; return T_EQ; }
"!="            { ECHO; return T_NE; }
">"             { ECHO; return T_GT; }
"<"             { ECHO; return T_LT; }
">="            { ECHO; return T_GE; }
"<="            { ECHO; return T_LE; }

{NUMBER}        { ECHO; yylval.lli = atoll(yytext); return T_NUM; }
{IDENTIFIER}    { ECHO; yylval.string = new std::string(yytext, yyleng); return T_IDENTIFIER; }

.               return T_ERROR;

%%


void run_lexer(FILE *file)
{
    cout << "Starting lexing...\n\n";

    yyin = file;

    while(yylex());

    cout << "\n";
}