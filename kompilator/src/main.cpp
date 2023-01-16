/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Main
*/


#include <iostream>
#include <fstream>

#include "./symtab.h"
#include "./astree.h"
#include "./asm.cpp"


using std::cout;


extern int yydebug;
extern void run_parser(FILE *file, vector<SymTabNode> &symbolTable, ASTree &programTree);
extern void yyerror(vector<SymTabNode> &symbolTable, ASTree &programTree, string s);


map<ASTree::TreeType, std::string> ASTreeTypesStrings = {
    {ASTree::kIdentifier,       "Identifier"},
    {ASTree::kNumber,           "Number"},
    {ASTree::kEqual,            "Equal"},
    {ASTree::kNotEqual,         "NotEqual"},
    {ASTree::kGreater,          "Greater"},
    {ASTree::kLess,             "Less"},
    {ASTree::kGreaterEqual,     "GreaterEqual"},
    {ASTree::kLessEqual,        "LessEqual"},
    {ASTree::kAddition,         "Add"},
    {ASTree::kSubtraction,      "Sub"},
    {ASTree::kMultiplication,   "Mul"},
    {ASTree::kDivision,         "Div"},
    {ASTree::kModulo,           "Mod"},
    {ASTree::kDeclarations,     "Declarations"},
    {ASTree::kParameters,       "Parameters"},
    {ASTree::kNewProcedure,     "NewProc"},
    {ASTree::kCallProcedure,    "CallProc"},
    {ASTree::kWrite,            "Write"},
    {ASTree::kRead,             "Read"},
    {ASTree::kCall,             "Call"},
    {ASTree::kAssign,           "Assign"},
    {ASTree::kRepeatLoop,       "Repeat"},
    {ASTree::kWhileLoop,        "While"},
    {ASTree::kIfCond,           "If"},
    {ASTree::kIfElseCond,       "IfElse"},
    {ASTree::kCommands,         "Commands"},
    {ASTree::kMain,             "Main"},
    {ASTree::kProcedures,       "Procedures"},
    {ASTree::kProgram,          "Program"}
};


void printPreorder(vector<SymTabNode> symbolTable, ASTree *tree, int depth)
{
    if (tree == NULL)
    {
        return;
    }

    std::cout << "DEPTH: " << depth;

    for (int i = 0; i <= depth; i++)
    {
        std::cout << "    ";
    }

    std::cout << ">> " << ASTreeTypesStrings[tree->mTreeType] 
              << "\t\tIndex: " << tree->mTreeMemoryIndex
              << "\tBranchesCount: " << tree->mTreeBranchesCount;
    if (tree->mTreeMemoryIndex != -1)
    {
        std::cout << "\t-> Var ID: " << symbolTable[tree->mTreeMemoryIndex].mNodeIdentifier
                  << "\tVar value: " << symbolTable[tree->mTreeMemoryIndex].mNodeValue;
    }
    std::cout << "\n";
    
    
    depth++;

    for (ASTree *subTree : tree->mTreeBranches)
    {
        printPreorder(symbolTable, subTree, depth);
    }
}


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "\n\t\033[31m[ERROR] Nie podano pliku źródłowego\033[0m\n";
        return 1;
    }

    vector<SymTabNode> symbolTable;
    ASTree programSyntax(ASTree::kProgram, -1, 0, {});
    vector<ASM> programCode;

    FILE *io = fopen(argv[1], "r");

    if (io)
    {
        run_parser(io, symbolTable, programSyntax);
        fclose(io);
    }
    else
    {
        string fileName(argv[1]);
        string errorMsg = "Nie udało się otworzyć pliku \'" + fileName + "\'"; 
        yyerror(symbolTable, programSyntax, errorMsg);
    }


    cout << "\n\n\nFROM MAIN: ";
      
    cout << "\n\nSymbol Table: ";
    for (SymTabNode node : symbolTable)
    {
        cout << "\n>> Variable -> type: " << node.mNodeType 
             << "\tID: " << node.mNodeIdentifier 
             << "\t\tValue: " << node.mNodeValue 
             << "\tIndex: " << node.mNodeIndex
             << "\tParamCount: " << node.mNodeParamCount; 
    }

    cout << "\n\nASTREE:\n";
    printPreorder(symbolTable, &programSyntax, 0);

    
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

 

    addConstants(programCode, symbolTable);
    generateCode(programCode, symbolTable, &programSyntax);

    pushInstruction(programCode, ASM::kHalt);


    cout << "\n\nAFTER CODEGEN:\n";


    saveCodeToFile(outputFile, programCode);


    for (SymTabNode node : symbolTable)
    {
        cout << "\n>> Variable -> type: " << node.mNodeType 
             << "\tID: " << node.mNodeIdentifier 
             << "\t\tValue: " << node.mNodeValue 
             << "\tIndex: " << node.mNodeIndex
             << "\tParamCount: " << node.mNodeParamCount; 
    }

    cout << "\n\n";


    outputFile.close();


    return 0;
}