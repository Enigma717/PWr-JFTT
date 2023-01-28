/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Implementacja drzewa składniowego AST
*/


#include "./astree.h"


//////////////////////////////
// Konstruktor klasy ASTree //
//////////////////////////////

ASTree::ASTree(ASTree::TreeType type, int index, int branchesCount, initializer_list<ASTree *> branches) : 
    mTreeType(type), mTreeMemoryIndex(index), mTreeBranchesCount(branchesCount)
{
    for (ASTree *branch : branches)
    {
        mTreeBranches.push_back(branch);
    }
}


///////////////////////////////////////
// Korzeń drzewa AST jako połączenie //
// poddrzew maina i procedur         //
///////////////////////////////////////

ASTree programRoot(ASTree *procs, ASTree *main)
{
    ASTree program(ASTree::kProgram, -1, 2, {procs, main});

    return program;
}


////////////////////////////////////////////
// Tworzenie nowych rozgałęzień w drzewie //
////////////////////////////////////////////

ASTree *newTreeValue(ASTree::TreeType type, int index)
{
    ASTree *value = new ASTree(type, index, 0, {});

    return value;
}


ASTree *newTreeBranch(ASTree::TreeType type)
{
    ASTree *branch = new ASTree(type, -1, 0, {});

    return branch;
}

ASTree *newTreeBranch(ASTree::TreeType type, ASTree *arg)
{
    ASTree *branch = new ASTree(type, -1, 1, {arg});

    return branch;
}

ASTree *newTreeBranch(ASTree::TreeType type, ASTree *firstArg, ASTree *secondArg)
{
    ASTree *branch = new ASTree(type, -1, 2, {firstArg, secondArg});
    
    return branch;
}

ASTree *newTreeBranch(ASTree::TreeType type, ASTree *firstArg, ASTree *secondArg, ASTree *thirdArg)
{
    ASTree *branch = new ASTree(type, -1, 3, {firstArg, secondArg, thirdArg});
    
    return branch;
}


///////////////////////////////////////////////////////////
// Dodawanie poddrzewa procedury do korzenia kProcedures //
///////////////////////////////////////////////////////////

ASTree *addProcedure(ASTree *procsRoot, ASTree *procedure, ASTree *commands)
{
    procedure->mTreeBranches.push_back(commands);
    procedure->mTreeBranchesCount++;

    procsRoot->mTreeBranches.push_back(procedure);
    procsRoot->mTreeBranchesCount++;

    return procsRoot;
}

ASTree *addProcedure(ASTree *procsRoot, ASTree *procedure, ASTree *variables, ASTree *commands)
{
    procedure->mTreeBranches.push_back(variables);
    procedure->mTreeBranchesCount++;
    procedure->mTreeBranches.push_back(commands);
    procedure->mTreeBranchesCount++;

    procsRoot->mTreeBranches.push_back(procedure);
    procsRoot->mTreeBranchesCount++;

    return procsRoot;
}


////////////////////////////////////////////////////////////
// Dodawanie poddrzewa zmiennej do korzenia kDeclarations //
////////////////////////////////////////////////////////////

ASTree *addDeclaration(ASTree *declsRoot, ASTree *variable)
{
    declsRoot->mTreeBranches.push_back(variable);
    declsRoot->mTreeBranchesCount++;

    return declsRoot;
}


///////////////////////////////////////////////////////
// Dodawanie poddrzewa komendy do korzenia kCommands //
///////////////////////////////////////////////////////

ASTree *addCommand(ASTree *commsRoot, ASTree *command)
{
    commsRoot->mTreeBranches.push_back(command);
    commsRoot->mTreeBranchesCount++;

    return commsRoot;
}
