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
    #include <map>

    #include "../symtab.cpp"
    #include "./parser.hpp"

    extern string currentProc;

    using std::cout;

    int yylex();

    extern int argCount;
    int scope = 0;
    int paramCount = 0;
%}


%option noyywrap
%option yylineno


%x COMMENT
%x PROC_HEAD
%x PROC_PARAMS
%x BEGIN_MAIN
%x VAR_DECLARE


IDENTIFIER                  [_a-z]+
NUMBER                      [0-9]+


%%
    /* Komentarze */
"["                         { ECHO; BEGIN(COMMENT); }
<COMMENT>[^]]               { ECHO; }
<COMMENT>"]"                { ECHO; BEGIN(0); }


    /* Białe znaki */
[ \t\n]                     { ECHO; }


    /* Tokeny słów kluczowych języka + znaki specjalne i operatory */
"PROCEDURE"                 { ECHO; BEGIN(PROC_HEAD); scope = symbolTable.size(); return yylval.token = T_PROCEDURE; }
"PROGRAM"                   { ECHO; BEGIN(BEGIN_MAIN); scope = symbolTable.size(); return yylval.token = T_PROGRAM; }
"IS"                        { ECHO; return yylval.token = T_IS; }
"VAR"                       { ECHO; BEGIN(VAR_DECLARE); return yylval.token = T_VAR; }
"BEGIN"                     { ECHO; return yylval.token = T_BEGIN; }
"END"                       { ECHO; return yylval.token = T_END; }
"IF"                        { ECHO; return yylval.token = T_IF; }
"THEN"                      { ECHO; return yylval.token = T_THEN; }
"ELSE"                      { ECHO; return yylval.token = T_ELSE; }
"ENDIF"                     { ECHO; return yylval.token = T_ENDIF; }
"WHILE"                     { ECHO; return yylval.token = T_WHILE; }
"DO"                        { ECHO; return yylval.token = T_DO; }
"ENDWHILE"                  { ECHO; return yylval.token = T_ENDWHILE; }
"REPEAT"                    { ECHO; return yylval.token = T_REPEAT; }
"UNTIL"                     { ECHO; return yylval.token = T_UNTIL; }
"READ"                      { ECHO; return yylval.token = T_READ; }
"WRITE"                     { ECHO; return yylval.token = T_WRITE; }


"("                         { ECHO; return yylval.token = T_LPAR; }
")"                         { ECHO; return yylval.token = T_RPAR; }
":="                        { ECHO; return yylval.token = T_ASSIGN; }
","                         { ECHO; return yylval.token = T_COMMA; }
";"                         { ECHO; return yylval.token = T_SEMICOLON; }

"+"                         { ECHO; return yylval.token = T_ADD; }
"-"                         { ECHO; return yylval.token = T_SUB; }
"*"                         { ECHO; return yylval.token = T_MUL; }
"/"                         { ECHO; return yylval.token = T_DIV; }
"%"                         { ECHO; return yylval.token = T_MOD; }

"="                         { ECHO; return yylval.token = T_EQ; }
"!="                        { ECHO; return yylval.token = T_NE; }
">"                         { ECHO; return yylval.token = T_GT; }
"<"                         { ECHO; return yylval.token = T_LT; }
">="                        { ECHO; return yylval.token = T_GE; }
"<="                        { ECHO; return yylval.token = T_LE; }


    /* Odczytanie nazwy procedury */
<PROC_HEAD>{IDENTIFIER}     { 
                                string procId(yytext, yyleng);
                                paramCount = 0;

                                if (seekNodeInSymTab(procId) == -1)
                                {
                                    yylval.tab_index = insertProcToSymTab(procId);
                                }
                                else
                                {
                                    string errorMsg = "Redeklaracja procedury \'" + procId + "\'";
                                    yyerror(errorMsg);
                                }

                                ECHO;
                                return T_IDENTIFIER; 
                            }
<PROC_HEAD>"("              { ECHO; BEGIN(PROC_PARAMS); return yylval.token = T_LPAR;  }


    /* Parametry formalne procedury */
<PROC_PARAMS>{IDENTIFIER}   {
                                string varId(yytext, yyleng);
                                paramCount++;
    
                                if (seekNodeInSymTab(varId, scope) == -1)
                                {
                                    yylval.tab_index = insertVarToSymTab(varId);
                                }
                                else
                                {
                                    string errorMsg = "Redeklaracja zmiennej \'" + varId + "\'";
                                    yyerror(errorMsg);
                                }

                                ECHO;
                                return T_IDENTIFIER; 
                            }
<PROC_PARAMS>","            { ECHO; return yylval.token = T_COMMA; }
<PROC_PARAMS>")"            { 
                                symbolTable[scope].mNodeParamCount = paramCount;

                                ECHO; 
                                BEGIN(0); 
                                return yylval.token = T_RPAR; 
                            }


    /* Odczytanie maina */
<BEGIN_MAIN>"IS"            { 
                                yylval.tab_index = insertProcToSymTab("main"); 

                                BEGIN(0);
                                return T_IS;
                            }


    /* Deklaracje zmiennych lokalnych */
<VAR_DECLARE>{IDENTIFIER}   {
                                string varId(yytext, yyleng);

                                if (seekNodeInSymTab(varId, scope) == -1)
                                {
                                    yylval.tab_index = insertVarToSymTab(varId);
                                }
                                else
                                {
                                    string errorMsg = "Redeklaracja zmiennej \'" + varId + "\'";
                                    yyerror(errorMsg);
                                }

                                ECHO;
                                return T_IDENTIFIER; 
                            }
<VAR_DECLARE>","            { ECHO; return yylval.token = T_COMMA; }
<VAR_DECLARE>"BEGIN"        { ECHO; yylineno++; BEGIN(0); return yylval.token = T_BEGIN; }


    /* Liczby */
{NUMBER}                    { 
                                string numId(yytext, yyleng);

                                int index = seekNodeInSymTab(numId);

                                if (index == -1)
                                {
                                    yylval.tab_index = insertNumToSymTab(numId);
                                }
                                else
                                {
                                    yylval.tab_index = index;
                                }


                                ECHO;   
                                return T_NUM; 
                            }


    /* Wywołanie funkcji */
{IDENTIFIER}"("             { 
                                argCount = 0;
    
                                string procId(yytext, yyleng);
                                procId.pop_back();

                                int index = seekNodeInSymTab(procId);

                                if (index != -1)
                                {
                                    yylval.tab_index = index;
                                }
                                else
                                {
                                    string errorMsg = "Użycie niezadeklarowanej procedury \'" + procId + "\'";
                                    yyerror(errorMsg);
                                }

                                ECHO;
                                return T_PROCCALL;
                            }

    /* Nazwy zmiennych */
{IDENTIFIER}                { 
                                string varId(yytext, yyleng);

                                int index = seekNodeInSymTab(varId, scope);
                                if (index != -1)
                                {
                                    yylval.tab_index = index;
                                }
                                else
                                {
                                    string errorMsg = "Użycie niezadeklarowanej zmiennej \'" + varId + "\'";
                                    yyerror(errorMsg);
                                }

                                ECHO;
                                return T_IDENTIFIER; 
                            }


    /* Pozostałe znaki */
.                           { yyerror("Nieznany symbol"); }

%%


void run_lexer(FILE *file)
{
    cout << "Starting lexing...\n\n";

    yyin = file;

    while(yylex());

    cout << "\n";
}