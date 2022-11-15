#include "test/*asdf*/header.h\"
#include <stdio.h>
#include <foo/*bar*/baz.h>

int foo();

int bar(); 


int g_global_var = 1;


int baz();


volatile int g_global;


int main(int argc, char ** argv)
{
    printf("/* foo bar\n");
    

    
    baz();
    

    baz();


    return 1;
}