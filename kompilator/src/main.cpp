/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Main
*/


#include <iostream>
#include <string>


using std::cout, std::string;


extern int yydebug;
extern void run_lexer(FILE *file);
extern void run_parser(FILE *file);
extern void yyerror(string s);


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

    if (io)
    {
        run_parser(io);

        fclose(io);
    }
    else
    {
        string fileName(argv[1]);
        string errorMsg = "Nie udało się otworzyć pliku \'" + fileName + "\'"; 
        yyerror(errorMsg);
    }    

    return 0;
}