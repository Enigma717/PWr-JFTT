%{
    #include <stdio.h>
    #include <stdbool.h>
    
    bool dox_cmt = false;
%}


INQUOTES                        "\"".*"\""
INCLUDE                         "#include"[ ]*((\"([^\"])*\")|(<([^>])*>))
SINGLECMT                       ("//"|"/").*(\\\n.*)*
MULTICMT                        "/*"([^*]|[*]+[^*/])*"*/"            
SINGLECMTDOX                    ("///"|"//!").*(\\\n.*)*
MULTICMTDOX                     "/*"("*"|"!")([^*]|[*]+[^*/])*"*/"            


%%


{INQUOTES}                      |
{INCLUDE}                       ECHO;
{SINGLECMTDOX}|{MULTICMTDOX}    {
                                    if (dox_cmt == true)
                                    {
                                        ECHO;
                                    }
                                    else
                                    {
                                        ;
                                    }
                                }
{SINGLECMT}                     |
{MULTICMT}                      ;


%%
  

int yywrap() {}

int main(int argc, char** argv) 
{
    if ((argc > 1) && (strcmp(argv[1], "dox") == 0))
    {
        dox_cmt = true;
    }
    
    yylex();


    return 0;
}