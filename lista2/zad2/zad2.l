%{
%}


QUOTELIT                        \\\"
QUOTE                           [\"]{1,3}
APOSTROPHE                      \'


%%


{QUOTELIT}                      |
{APOSTROPHE}.*{APOSTROPHE}      |
{QUOTE}([^\"])*{QUOTE}       ECHO;

#.*                             ;


%%
  

int yywrap() {}

int main() 
{
    yylex();


    return 0;
}