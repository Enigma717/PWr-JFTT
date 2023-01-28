/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Klasa drzewa składniowego AST (header)
*/


#ifndef ASTREE_H
#define ASTREE_H


#include <vector>
#include <initializer_list>

using std::vector, std::initializer_list;


class ASTree {
public:
    enum TreeType {
        kIdentifier     = 1, 
        kNumber         = 2, 
        
        kEqual          = 3, 
        kNotEqual       = 4, 
        kGreater        = 5, 
        kLess           = 6, 
        kGreaterEqual   = 7, 
        kLessEqual      = 8,

        kAddition       = 9,
        kSubtraction    = 10, 
        kMultiplication = 11, 
        kDivision       = 12, 
        kModulo         = 13,

        kDeclarations   = 14, 
        kParameters     = 15, 
        kNewProcedure   = 16, 
        kCallProcedure  = 17,

        kWrite          = 18, 
        kRead           = 19, 
        kCall           = 20, 
        kAssign         = 21,
        kRepeatLoop     = 22, 
        kWhileLoop      = 23,
        kIfCond         = 24, 
        kIfElseCond     = 25,

        kCommands       = 26,
        
        kMain           = 27, 
        kProcedures     = 28, 
        kProgram        = 29  
    };

    TreeType         mTreeType;
    int              mTreeMemoryIndex;
    int              mTreeBranchesCount;
    vector<ASTree *> mTreeBranches;

    ASTree(TreeType type, int index, int branchesCount, initializer_list<ASTree *> branches);
};


ASTree programRoot(ASTree *procs, ASTree *main);

ASTree *newTreeValue(ASTree::TreeType type, int index);
ASTree *newTreeBranch(ASTree::TreeType type);
ASTree *newTreeBranch(ASTree::TreeType type, ASTree *arg);
ASTree *newTreeBranch(ASTree::TreeType type, ASTree *firstArg, ASTree *secondArg);
ASTree *newTreeBranch(ASTree::TreeType type, ASTree *firstArg, ASTree *secondArg, ASTree *thirdArg);

ASTree *addProcedure(ASTree *procsRoot, ASTree *procedure, ASTree *commands);
ASTree *addProcedure(ASTree *procsRoot, ASTree *procedure, ASTree *variables, ASTree *commands);

ASTree *addDeclaration(ASTree *declsRoot, ASTree *variable);

ASTree *addCommand(ASTree *commsRoot, ASTree *command);


#endif