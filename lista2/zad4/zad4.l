%{
    #include <iostream>
    #include <cmath>
    #include <string>
    #include <stack>

    using namespace std;

    stack<int> operators;
    int val1, val2;
    char symbol;
%}


%x ARGSERROR                
%x ZEROERROR                
%x SYMBERROR                


%%


\-?[0-9]+               {
                            ECHO;
                            operators.push(stoi(yytext));
                        }

[\+\-\*\^/%]            {
                            ECHO;

                            if (operators.size() >= 2)
                            {
                                val2 = operators.top();
                                operators.pop(); 
                                val1 = operators.top();
                                operators.pop(); 

                                REJECT;
                            }
                            else
                            {
                                BEGIN(ARGSERROR);
                            }
                        }

[/%]                    {
                            if (val2 == 0)
                            {
                                BEGIN(ZEROERROR);
                            }
                            else
                            {
                                REJECT;
                            }
                        }

[^0-9\+\-\*\^/%\n\t ]      {
                            ECHO;
                            symbol = yytext[0];
                            BEGIN(SYMBERROR);
                        }

\+                      operators.push(val1 + val2);
\-                      operators.push(val1 - val2);
\*                      operators.push(val1 * val2);
\^                      operators.push((int)pow(val1, val2));
\/                      operators.push(val1 / val2);
\%                      operators.push(val1 % val2);

\n                      {
                            ECHO;

                            if (operators.size() == 1)
                            {
                                printf("= %d\n", operators.top());
                                operators.pop();
                            }

                            if (operators.size() > 1)
                            {
                                printf("Błąd: za mała liczba operatorów\n");
                                operators = *(new stack<int>);
                            }
                        }

<ARGSERROR><<EOF>>      {
                            printf("\nBłąd: za mała liczba argumentów\n");
                            return 1;
                        }
<ARGSERROR>\n           {
                            printf("\nBłąd: za mała liczba argumentów");
                            operators = *(new stack<int>);
                            BEGIN(0);
                        }

<ZEROERROR><<EOF>>      {
                            printf("\nBłąd: dzielenie przez 0\n");
                            return 1;
                        }
<ZEROERROR>\n           {
                            printf("\nBłąd: dzielenie przez 0");
                            ECHO;
                            operators = *(new stack<int>);
                            BEGIN(0);
                        }

<SYMBERROR><<EOF>>      {
                            printf("\nBłąd: zły symbol \"%c\"\n", symbol);
                            return 1;
                        }
<SYMBERROR>\n           {
                            printf("\nBłąd: zły symbol \"%c\"", symbol);
                            ECHO;
                            operators = *(new stack<int>);
                            BEGIN(0);
                        }

<<EOF>>                 {
                            if (operators.size() == 1)
                            {
                                printf("\n= %d\n", operators.top());
                                operators.pop();
                            }

                            return 1;
                        }

%%


int yywrap() {}

int main(int argc, char** argv) 
{
    yylex();


    return 0;
}