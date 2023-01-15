/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Implementacja drzewa składniowego
*/



#include "./astree.h"


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

ASTree startProgram(ASTree *procs, ASTree *main)
{
    ASTree program(ASTree::kProgram, -1, 2, {procs, main});

    return program;
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
