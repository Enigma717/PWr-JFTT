%{
    #include "zad1.tab.h"

    int yylex();
%}


%option noyywrap


%%

^#(.|\\\n)*\n   ;
\\\n            ;
[ \t]+          ;
[0-9]+          { 
                    yylval = atoi(yytext);
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

%%
