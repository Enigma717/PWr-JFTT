/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Main
*/


#include <iostream>
#include <fstream>

#include "./symtab/symtab.h"
#include "./ast/astree.h"
#include "./asm/asm.cpp"


using std::cout;


const string kColorReset  = "\033[0m";
const string kColorRed    = "\033[31m";
const string kColorGreen  = "\033[32m";
const string kColorBlue   = "\033[34m";

extern void run_parser(FILE *file, vector<SymTabNode> &symbolTable, ASTree &programTree);


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << kColorRed << "\n\t[ERROR] " << kColorGreen << "Nie podano pliku źródłowego\n\n" << kColorReset;
        std::cout << kColorBlue << "\n\t[INFO] " << kColorGreen << "Prawidłowe wywołanie: ./compiler [input] [output (default = ./output.mr)]" << kColorReset;
        return 1;
    }

    ASTree programSyntax(ASTree::kProgram, -1, 0, {});
    vector<SymTabNode> symbolTable;
    vector<ASM> programCode;


    // Rozpoczęcie parsowania

    FILE *io = fopen(argv[1], "r");
    
    string fileName(argv[1]);

    if (!io)
    {
        std::cerr << kColorRed << "\n\t[ERROR] " << kColorGreen << "Nie udało się otworzyć pliku \'" << fileName<< "\'\n\n" << kColorReset;
        return 1;
    }

    std::cout << kColorBlue << "\n\t[INFO] " << kColorGreen << "Rozpoczęcie kompilacji pliku: " << fileName << kColorReset;
    std::cout << kColorBlue << "\n\n\t\t[INFO] " << kColorGreen << "Rozpoczęcie parsowania..." << kColorReset;
    
    run_parser(io, symbolTable, programSyntax);
    fclose(io);
    
    std::cout << kColorBlue << "\n\t\t\t[INFO] " << kColorGreen << "Parsowanie ukończone pomyślnie" << kColorReset;

    
    // Rozpoczęcie generowania kodu

    std::string oFileName;
    std::ofstream outputFile;

    if (argv[2])
    {
        oFileName = argv[2];
    }
    else
    {
        oFileName = "./output.mr";
    }

    outputFile.open(oFileName);

    std::cout << kColorBlue << "\n\n\t\t[INFO] " << kColorGreen << "Rozpoczęcie generowania kodu..." << kColorReset;

    addConstants(programCode, symbolTable);
    generateCode(programCode, symbolTable, &programSyntax, programCode.size());
    pushInstruction(programCode, ASM::kHalt);
    std::cout << kColorBlue << "\n\t\t\t[INFO] " << kColorGreen << "Generowanie kodu ukończone pomyślnie\n" << kColorReset;
    saveCodeToFile(outputFile, programCode);

    outputFile.close();

    std::cout << kColorBlue << "\n\t[INFO] " << kColorGreen << "Kompilacja ukończona pomyślnie" << kColorReset;
    std::cout << kColorBlue << "\n\n\n\t[INFO] " << kColorGreen << "Kod wynikowy został zapisany w pliku: " << oFileName << "\n\n" << kColorReset;


    return 0;
}