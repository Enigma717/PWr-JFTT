%{
    #include "zad1bison.h"

    int yylex();
%}


%option noyywrap


%%

[ \t]           ;
^#(.|\\\n)*\n   ;
\\\n            ;
[0-9]+          { 
                    yylval.lli = atoi(yytext);
                    return NUM; 
                }

\+              return '+';
\-              return '-';
\*              return '*';
\/              return '/';
\^              return '^';
\(              return '(';
\)              return ')';
\n              return LF;
.               return SYNTAX_ERROR;

%%
