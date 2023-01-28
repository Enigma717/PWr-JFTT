    /*
        Autor:      Marek Traczyński (261748)
        Kurs:       Języki Formalne i Techniki Translacji
        Projekt:    Kompilator
        Opis pliku: Lexer
    */


%{
    #include "../symtab/symtab.cpp"
    #include "./parser.hpp"

    extern string currentProc;
    extern int argCount;

    int scope = 0;
    int paramCount = 0;
    int loopDepth = 0;
%}


%option noyywrap
%option yylineno


%x COMMENT
%x PHEAD_COMMENT
%x PARAMS_COMMENT
%x VAR_COMMENT

%x PROC_HEAD
%x PROC_PARAMS
%x BEGIN_MAIN
%x VAR_DECLARE


IDENTIFIER                  [_a-z]+
NUMBER                      [0-9]+


%%
    /* Komentarze */
"["                         { BEGIN(COMMENT); }
<COMMENT>[^]]               { ;               }
<COMMENT>"]"                { BEGIN(0);       }


    /* Białe znaki */
[ \t\n\r]                   { ; }


    /* Tokeny słów kluczowych języka + znaki specjalne i operatory */
"PROCEDURE"                 { scope = symbolTable.size(); BEGIN(PROC_HEAD);   return yylval.token = T_PROCEDURE; }
"PROGRAM"                   { scope = symbolTable.size(); BEGIN(BEGIN_MAIN);  return yylval.token = T_PROGRAM;   }
"IS"                        {                                                 return yylval.token = T_IS;        }
"VAR"                       {                             BEGIN(VAR_DECLARE); return yylval.token = T_VAR;       }
"BEGIN"                     {                                                 return yylval.token = T_BEGIN;     }
"END"                       {                                                 return yylval.token = T_END;       }
"IF"                        {                                                 return yylval.token = T_IF;        }
"THEN"                      {                                                 return yylval.token = T_THEN;      }
"ELSE"                      {                                                 return yylval.token = T_ELSE;      }
"ENDIF"                     {                                                 return yylval.token = T_ENDIF;     }
"WHILE"                     { loopDepth++;                                    return yylval.token = T_WHILE;     }
"DO"                        {                                                 return yylval.token = T_DO;        }
"ENDWHILE"                  { loopDepth--;                                    return yylval.token = T_ENDWHILE;  }
"REPEAT"                    { loopDepth++;                                    return yylval.token = T_REPEAT;    }
"UNTIL"                     { loopDepth--;                                    return yylval.token = T_UNTIL;     }
"READ"                      {                                                 return yylval.token = T_READ;      }
"WRITE"                     {                                                 return yylval.token = T_WRITE;     }


"("                         { return yylval.token = T_LPAR;      }
")"                         { return yylval.token = T_RPAR;      }
":="                        { return yylval.token = T_ASSIGN;    }
","                         { return yylval.token = T_COMMA;     }
";"                         { return yylval.token = T_SEMICOLON; }

"+"                         { return yylval.token = T_ADD; }
"-"                         { return yylval.token = T_SUB; }
"*"                         { return yylval.token = T_MUL; }
"/"                         { return yylval.token = T_DIV; }
"%"                         { return yylval.token = T_MOD; }

"="                         { return yylval.token = T_EQ; }
"!="                        { return yylval.token = T_NE; }
">"                         { return yylval.token = T_GT; }
"<"                         { return yylval.token = T_LT; }
">="                        { return yylval.token = T_GE; }
"<="                        { return yylval.token = T_LE; }


    /* Komentarze i białe znaki w deklaracji procedury */
    /* Nieeleganckie rozwiązanie, ponieważ powtarza się */
    /* dla każdej sekcji, ale nie mam innego pomysłu/czasu */
<PROC_HEAD>"["              { BEGIN(PHEAD_COMMENT); }
<PHEAD_COMMENT>[^]]         { ;                     }                         
<PHEAD_COMMENT>"]"          { BEGIN(PROC_HEAD);     }
<PROC_HEAD>[ \t\n\r]        { ;                     }
    /* Odczytanie nazwy procedury */
<PROC_HEAD>{IDENTIFIER}     {
                                string procId(yytext, yyleng);
                                paramCount = 0;

                                if (seekNodeInSymTab(symbolTable, procId) == -1)
                                {
                                    yylval.tab_index = insertProcToSymTab(symbolTable, procId);
                                }
                                else
                                {
                                    string errorMsg = "Redeklaracja procedury \'" + procId + "\'";
                                    yyerror(symbolTable, programTree, errorMsg);
                                }

                                return T_IDENTIFIER; 
                            }
<PROC_HEAD>"("              { BEGIN(PROC_PARAMS); return yylval.token = T_LPAR; }


    /* Komenatrze i białe znaki w deklaracjach parametrów */
<PROC_PARAMS>"["            { BEGIN(PARAMS_COMMENT); }
<PARAMS_COMMENT>[^]]        { ;                      }                         
<PARAMS_COMMENT>"]"         { BEGIN(PROC_PARAMS);    }
<PROC_PARAMS>[ \t\n\r]      { ;                      }
    /* Parametry formalne procedury */
<PROC_PARAMS>{IDENTIFIER}   {
                                string parId(yytext, yyleng);
                                paramCount++;
    
                                if (seekNodeInSymTab(symbolTable, parId, scope) == -1)
                                {
                                    yylval.tab_index = insertParToSymTab(symbolTable, parId);
                                }
                                else
                                {
                                    string errorMsg = "Redeklaracja zmiennej \'" + parId + "\'";
                                    yyerror(symbolTable, programTree, errorMsg);
                                }

                                return T_IDENTIFIER; 
                            }
<PROC_PARAMS>","            { return yylval.token = T_COMMA; }
<PROC_PARAMS>")"            { 
                                symbolTable[scope].mNodeParamCount = paramCount;

                                BEGIN(0); 
                                return yylval.token = T_RPAR; 
                            }


    /* Odczytanie maina */
<BEGIN_MAIN>"IS"            { 
                                yylval.tab_index = insertProcToSymTab(symbolTable, "main"); 

                                BEGIN(0);
                                return T_IS;
                            }


    /* Komenatrze i białe znaki w deklaracjach zmiennych */
<VAR_DECLARE>"["            { BEGIN(VAR_COMMENT); }
<VAR_COMMENT>[^]]           { ;                   }                         
<VAR_COMMENT>"]"            { BEGIN(VAR_DECLARE); }   
<VAR_DECLARE>[ \t\n\r]      { ;                   }
    /* Deklaracje zmiennych lokalnych */
<VAR_DECLARE>{IDENTIFIER}   {
                                string varId(yytext, yyleng);
                                if (seekNodeInSymTab(symbolTable, varId, scope) == -1)
                                {
                                    yylval.tab_index = insertVarToSymTab(symbolTable, varId);
                                }
                                else
                                {
                                    string errorMsg = "Redeklaracja zmiennej \'" + varId + "\'";
                                    yyerror(symbolTable, programTree, errorMsg);
                                }

                                return T_IDENTIFIER; 
                            }
<VAR_DECLARE>","            {           return yylval.token = T_COMMA; }
<VAR_DECLARE>"BEGIN"        { yylineno++; BEGIN(0); return yylval.token = T_BEGIN; }


    /* Liczby */
{NUMBER}                    { 
                                string numId(yytext, yyleng);

                                int index = seekNodeInSymTab(symbolTable, numId);

                                if (index == -1)
                                {
                                    yylval.tab_index = insertNumToSymTab(symbolTable, numId);
                                }
                                else
                                {
                                    yylval.tab_index = index;
                                }

                                return T_NUM; 
                            }


    /* Wywołanie funkcji */
{IDENTIFIER}"("             { 
                                argCount = 0;
    
                                string procId(yytext, yyleng);
                                procId.pop_back();

                                int index = seekNodeInSymTab(symbolTable, procId);

                                if (index != -1)
                                {
                                    yylval.tab_index = index;
                                }
                                else
                                {
                                    string errorMsg = "Użycie niezadeklarowanej procedury \'" + procId + "\'";
                                    yyerror(symbolTable, programTree, errorMsg);
                                }

                                return T_PROCCALL;
                            }

    /* Nazwy zmiennych */
{IDENTIFIER}                { 
                                string varId(yytext, yyleng);

                                int index = seekNodeInSymTab(symbolTable, varId, scope);
                                if (index != -1)
                                {
                                    yylval.tab_index = index;
                                }
                                else
                                {
                                    string errorMsg = "Użycie niezadeklarowanej zmiennej \'" + varId + "\'";
                                    yyerror(symbolTable, programTree, errorMsg);
                                }

                                return T_IDENTIFIER; 
                            }


    /* Pozostałe znaki */
.                           { yyerror(symbolTable, programTree, "Nieznany symbol"); }

%%

