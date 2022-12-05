%{
    #include <stdio.h>
    #include <string.h>
    #include <stdbool.h>
    #include "field.c"
    

    int yylex();
    void yyerror(char const *s);

    bool error = false;
    char *result_rpn = "";
%}

%token NUM
%token LF

%left '-' '+'
%left '*' '/'
%precedence NEG
%right '^'


%%

input:
    %empty
    | input line
    ;

line: 
    LF
    | expression LF     {
                            if(error == false)
                            {
                                printf("%s\n", result_rpn); 
                                printf("Wynik: %d\n", $1); 
                            } 

                            error = false;
                            strcpy(result_rpn, "");
                        }
    | error LF          {

                            yyerror("Błąd składni.");
                            
                            error = false;
                            strcpy(result_rpn, "");
                        }
    ;

expression:
    value                               {
                                            int len = snprintf(NULL, 0, "%d ", $1);
                                            char *token = malloc(len + 1);

                                            snprintf(token, len + 1, "%d ", $1);
                                            strncat(result_rpn, token, len + 1);

                                            free(token);
                                        }       
    | expression '+' expression         { strncat(result_rpn, "+ ", 3); $$ = f_add($1, $3, ORDER); }
    | expression '-' expression         { strncat(result_rpn, "- ", 3); $$ = f_sub($1, $3, ORDER); }
    | expression '*' expression         { strncat(result_rpn, "* ", 3); $$ = f_mul($1, $3, ORDER); }
    | expression '/' expression         { 
                                            if($3 == 0)
                                            {
                                                yyerror("Dzielenie przez 0.");
                                            }
                                            else
                                            {
                                                strncat(result_rpn, "/ ", 3); 
                                                $$ = f_div($1, $3, ORDER);
                                            }
                                        }
    | expression '^' exponent           { strncat(result_rpn, "^ ", 3); $$ = f_pow($1, $3, ORDER); }
    | '-' '(' expression ')' %prec NEG  { strncat(result_rpn, "~ ", 3); $$ = f_sub(0, $3, ORDER);  }
    | '(' expression ')'                { $$ = $2;                                          }
    ;

value:
    NUM                     { $$ = $1 % ORDER;   }
    | '-' value %prec NEG   { $$ = f_sub(0, $2, ORDER); }
    ;


exponent:
    exp_value                           {
                                            int len = snprintf(NULL, 0, "%d ", $1);
                                            char *token = malloc(len + 1);

                                            snprintf(token, len + 1, "%d ", $1);
                                            strncat(result_rpn, token, len + 1);

                                            free(token);
                                        }       
    | exponent '+' exponent             { strncat(result_rpn, "+ ", 3); $$ = f_add($1, $3, ORDER - 1); }
    | exponent '-' exponent             { strncat(result_rpn, "- ", 3); $$ = f_sub($1, $3, ORDER - 1); }
    | exponent '*' exponent             { strncat(result_rpn, "* ", 3); $$ = f_mul($1, $3, ORDER - 1); }
    | exponent '/' exponent             { 
                                            if($3 == 0)
                                            {
                                                yyerror("Wykładnik nie jest odwracalny w modulo 1234576");
                                            }
                                            else
                                            {
                                                strncat(result_rpn, "/ ", 3); 
                                                $$ = f_div($1, $3, ORDER - 1);
                                            }
                                        }
    | exponent '^' exponent             { strncat(result_rpn, "^ ", 3); $$ = f_pow($1, $3, ORDER - 1); }
    | '-' '(' exponent ')' %prec NEG    { strncat(result_rpn, "~ ", 3); $$ = f_sub(0, $3, ORDER - 1);  }
    | '(' exponent ')'                  { $$ = $2;                                          }
    ;

exp_value:
    NUM                         { $$ = $1 % (ORDER - 1);   }
    | '-' exp_value %prec NEG   { $$ = f_sub(0, $2, ORDER - 1); }

%%


void yyerror(char const *s)
{
    error = true;
    printf("%s\n", s);
}


int main()
{
    result_rpn = malloc(100);

    yyparse();

    printf("EOF - exitting the app\n");

    free(result_rpn);

    return 0;
}