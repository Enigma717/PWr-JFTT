%{
    #include <stdbool.h>
    
    bool first_line = true;
    int lines, words = 0; 
%}


%s LINEFEED
%s WORDS
%s SPACE

WORD                        [^ \t\n]+
WHITESPACES                 [ \t]+


%%


    BEGIN(LINEFEED);

<LINEFEED><<EOF>>           return 0;
<LINEFEED>{WHITESPACES}     ;
<LINEFEED>\n                ;
<LINEFEED>{WORD}            {
                                if (first_line == false)
                                {
                                    printf("\n");    
                                }
                                else
                                {
                                    first_line = false;
                                }

                                ECHO;
                                lines++;
                                words++;
                                BEGIN(WORDS);
                            }


<WORDS>{WHITESPACES}        BEGIN(SPACE);
<WORDS>\n                   BEGIN(LINEFEED);
<WORDS><<EOF>>              return 0;

<SPACE>{WORD}               {
                                printf(" ");
                                ECHO;
                                words++;
                                BEGIN(WORDS);
                            }
<SPACE>\n                   BEGIN(LINEFEED);


%%
  

int yywrap() {}

int main() 
{
    yylex();

    printf("\nLines: %d | Words: %d\n", lines, words);
    

    return 0;
}