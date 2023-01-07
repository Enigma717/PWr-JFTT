#include <iostream>


using namespace std;


extern void run_lexer(FILE *file);


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cout << "No file\n";
        return 1;
    }


    FILE *io = fopen(argv[1], "r");

    run_lexer(io);

    fclose(io);
    

    return 0;
}