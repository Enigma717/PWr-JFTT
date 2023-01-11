/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Main
*/

#include <iostream>


using std::cout;


extern int yydebug;
extern void run_lexer(FILE *file);
extern void run_parser(FILE *file);


int main(int argc, char** argv)
{
    #if YYDEBUG
        yydebug = 1;
    #endif


    if (argc < 2)
    {
        cout << "No file\n";
        return 1;
    }


    FILE *io = fopen(argv[1], "r");

    run_parser(io);

    fclose(io);
    

    return 0;
}