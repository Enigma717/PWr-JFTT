/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Implementacja drzewa składniowego
*/


#include "./astree.h"


extern vector<SymTabNode> symbolTable;

vector<ASTree *> ASTreeTable;

map<ASTree::TreeType, std::string> enumStrings = {
    {ASTree::kIdentifier, "Identifier"},
    {ASTree::kNumber, "Number"},
    {ASTree::kEqual,  "Equal"},
    {ASTree::kNotEqual,  "NotEqual"},
    {ASTree::kGreater,  "Greater"},
    {ASTree::kLess,  "Less"},
    {ASTree::kGreaterEqual,  "GreaterEqual"},
    {ASTree::kLessEqual, "LessEqual"},
    {ASTree::kAddition, "Add"},
    {ASTree::kSubtraction,  "Sub"},
    {ASTree::kMultiplication,  "Mul"},
    {ASTree::kDivision,  "Div"},
    {ASTree::kModulo, "Mod"},
    {ASTree::kDeclarations,  "Declarations"},
    {ASTree::kNewProcedure,  "NewProc"},
    {ASTree::kCallProcedure, "CallProc"},
    {ASTree::kWrite, "Write"},
    {ASTree::kRead, "Read"},
    {ASTree::kCall, "Call"},
    {ASTree::kAssign, "Assign"},
    {ASTree::kRepeatLoop,  "Repeat"},
    {ASTree::kWhileLoop, "While"},
    {ASTree::kIfCond, "If"},
    {ASTree::kIfElseCond, "IfElse"},
    {ASTree::kCommand, "Command"},
    {ASTree::kMain, "Main"},
    {ASTree::kProcedures, "Procedures"},
    {ASTree::kProgram, "Program"}
};

void printPreorder(ASTree *tree, int depth)
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

    if (tree->mTreeType == ASTree::kNumber || tree->mTreeType == ASTree::kIdentifier )
    {
        std::cout << ">> " << enumStrings[tree->mTreeType] 
                  << "\t\tIndex: " << tree->mTreeMemoryIndex
                  << "\t-> Var ID: " << symbolTable[tree->mTreeMemoryIndex].mNodeIdentifier
                  << "\tVar value: " << symbolTable[tree->mTreeMemoryIndex].mNodeValue << "\n";
    }
    else
    {
        std::cout << ">> " << enumStrings[tree->mTreeType] << "\n"; 
    }

    depth++;

    for (ASTree *subTree : tree->mTreeBranches)
    {
        printPreorder(subTree, depth);
    }
}



ASTree::ASTree(ASTree::TreeType type, int index, int branchesCount, initializer_list<ASTree *> subTrees) : 
    mTreeType(type), mTreeMemoryIndex(index), mTreeBranchesCount(branchesCount)
{
    for (ASTree *tree : subTrees)
    {
        mTreeBranches.push_back(tree);
    }
}


ASTree *newTreeValue(ASTree::TreeType type, int index)
{
    ASTree *value = new ASTree(type, index, 0, {});

    return value;
}

ASTree *newTreeStatement(ASTree::TreeType type, ASTree *arg)
{
    ASTree *statement = new ASTree(type, -1, 1, {arg});

    return statement;
}

ASTree *newTreeStatement(ASTree::TreeType type, ASTree *firstArg, ASTree *secondArg)
{
    ASTree *statement = new ASTree(type, -1, 2, {firstArg, secondArg});
    
    return statement;
}

ASTree *addDeclarations(ASTree *proc, ASTree *var)
{
    proc->mTreeBranches.push_back(var);

    return proc;
}

