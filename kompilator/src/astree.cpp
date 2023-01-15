/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Implementacja drzewa składniowego
*/


#include "./astree.h"


extern vector<SymTabNode> symbolTable;

ASTree *programASTree;

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
    {ASTree::kParameters,  "Parameters"},
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
    {ASTree::kCommands, "Commands"},
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

    std::cout << ">> " << enumStrings[tree->mTreeType] 
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
        printPreorder(subTree, depth);
    }
}

////////////////////////////////////

ASTree::ASTree(ASTree::TreeType type, int index, int branchesCount, initializer_list<ASTree *> subTrees) : 
    mTreeType(type), mTreeMemoryIndex(index), mTreeBranchesCount(branchesCount)
{
    for (ASTree *tree : subTrees)
    {
        mTreeBranches.push_back(tree);
    }
}


////////////////////////////////////

void startProgram(ASTree *procs, ASTree *main)
{
    ASTree *program = new ASTree(ASTree::kProgram, -1, 2, {procs, main});

    programASTree = program;
}

////////////////////////////////////

ASTree *newTreeValue(ASTree::TreeType type, int index)
{
    ASTree *value = new ASTree(type, index, 0, {});

    return value;
}


ASTree *newTreeBranch(ASTree::TreeType type)
{
    ASTree *statement = new ASTree(type, -1, 0, {});

    return statement;
}

ASTree *newTreeBranch(ASTree::TreeType type, ASTree *arg)
{
    ASTree *statement = new ASTree(type, -1, 1, {arg});

    return statement;
}

ASTree *newTreeBranch(ASTree::TreeType type, ASTree *firstArg, ASTree *secondArg)
{
    ASTree *statement = new ASTree(type, -1, 2, {firstArg, secondArg});
    
    return statement;
}

////////////////////////////////////

ASTree *addProcedure(ASTree *procs, ASTree *head, ASTree *comms)
{
    head->mTreeBranches.push_back(comms);
    head->mTreeBranchesCount++;

    procs->mTreeBranches.push_back(head);
    procs->mTreeBranchesCount++;

    return procs;
}

ASTree *addProcedure(ASTree *procs, ASTree *head, ASTree *vars, ASTree *comms)
{
    head->mTreeBranches.push_back(vars);
    head->mTreeBranchesCount++;
    head->mTreeBranches.push_back(comms);
    head->mTreeBranchesCount++;

    procs->mTreeBranches.push_back(head);
    procs->mTreeBranchesCount++;

    return procs;
}

ASTree *addDeclaration(ASTree *head, ASTree *var)
{
    head->mTreeBranches.push_back(var);
    head->mTreeBranchesCount++;

    return head;
}

ASTree *addCommand(ASTree *commands, ASTree *command)
{
    commands->mTreeBranches.push_back(command);
    commands->mTreeBranchesCount++;

    return commands;
}
