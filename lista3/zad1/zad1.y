%{
    #include <stdio.h>
    #include <string.h>
    #include "field.c"
    

    extern int yylex();
    extern int yyparse();
    int yyerror(char const *s);


    char *result_rpn = "\0";
    char *error_message = "\0";
%}


%union {
    long long int lli;
}

%type <lli> expression exponent value exp_value

%token <lli> NUM
%token LF
%token SYNTAX_ERROR


%left '-' '+'
%left '*' '/'
%precedence NEG
%right '^'


%%

input:
    %empty          { printf(">> "); }    
    | input line    
    ;

line:
    LF                  { printf(">> "); }
    | expression LF     {
                            printf("|-> RPN   : %s\n", result_rpn); 
                            printf("\\-> Wynik : %d\n", $1); 
                            printf("\n>> ");

                            strncpy(result_rpn, "\0", 1);
                        }
    | error LF      {
                        if(strcmp(error_message, "\0") == 0)
                        {
                            strncpy(error_message, "Błąd składni.", 17);
                        }                            
                        printf("ERROR: %s\n", error_message);
                        printf("\n>> ");

                        strncpy(result_rpn, "\0", 1);
                        strncpy(error_message, "\0", 1);
                    }
    ;

expression:
    value                               {
                                            int len = snprintf(NULL, 0, "%d ", $1);
                                            char *token = malloc(len + 1);

                                            snprintf(token, len + 1, "%d ", $1);
                                            strncat(result_rpn, token, len + 1);

                                            free(token);

                                            $$ = $1;
                                        }       
    | expression '+' expression         { strncat(result_rpn, "+ ", 3); $$ = f_add($1, $3, ORDER); }
    | expression '-' expression         { strncat(result_rpn, "- ", 3); $$ = f_sub($1, $3, ORDER); }
    | expression '*' expression         { strncat(result_rpn, "* ", 3); $$ = f_mul($1, $3, ORDER); }
    | expression '/' expression         { 
                                            if($3 == 0)
                                            {
                                                strncpy(error_message, "Dzielenie przez 0.", 19);
                                                YYERROR;
                                            }
                                            else
                                            {
                                                strncat(result_rpn, "/ ", 3); 
                                                $$ = f_div($1, $3, ORDER);
                                            }
                                        }
    | '(' expression ')'                { $$ = $2;                                                 }
    | '-' '(' expression ')' %prec NEG  { strncat(result_rpn, "~ ", 3); $$ = f_sub(0, $3, ORDER);  }
    | expression '^' exponent           { strncat(result_rpn, "^ ", 3); $$ = f_pow($1, $3, ORDER); }
    ;

value:
    NUM                     { $$ = $1 % ORDER;          }
    | '-' value %prec NEG   { $$ = f_sub(0, $2, ORDER); }
    ;


exponent:
    exp_value                           {
                                            int len = snprintf(NULL, 0, "%d ", $1);
                                            char *token = malloc(len + 1);

                                            snprintf(token, len + 1, "%d ", $1);
                                            strncat(result_rpn, token, len + 1);

                                            free(token);

                                            $$ = $1;
                                        }       
    | exponent '+' exponent             { strncat(result_rpn, "+ ", 3); $$ = f_add($1, $3, ORDER - 1); }
    | exponent '-' exponent             { strncat(result_rpn, "- ", 3); $$ = f_sub($1, $3, ORDER - 1); }
    | exponent '*' exponent             { strncat(result_rpn, "* ", 3); $$ = f_mul($1, $3, ORDER - 1); }
    | exponent '/' exponent             { 
                                            if($3 == 0)
                                            {
                                                strncpy(error_message, "Dzielenie przez 0.", 19);
                                                YYERROR;
                                            }
                                            else
                                            {
                                                strncat(result_rpn, "/ ", 3); 
                                                $$ = f_div($1, $3, ORDER - 1);
                                            }
                                        }
    | '(' exponent ')'                  { $$ = $2;                                                     }
    | '-' '(' exponent ')' %prec NEG    { strncat(result_rpn, "~ ", 3); $$ = f_sub(0, $3, ORDER - 1);  }
    | exponent '^' exponent             { strncat(result_rpn, "^ ", 3); $$ = f_pow($1, $3, ORDER - 1); }
    ;

exp_value:
    NUM                         { $$ = $1 % (ORDER - 1);        }
    | '-' exp_value %prec NEG   { $$ = f_sub(0, $2, ORDER - 1); }
    ;

%%


int yyerror(char const *s)
{
    return 1;
}


int main()
{
    result_rpn = malloc(256);
    error_message = malloc(26);

    yyparse();

    printf("EOF - exitting the app\n");

    free(error_message);
    free(result_rpn);

    return 0;
}