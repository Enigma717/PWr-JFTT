/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Implementacja drzewa składniowego (header)
*/


#ifndef ASTREE_H
#define ASTREE_H


#include <string>
#include <map>
#include <vector>
#include <initializer_list>

using std::string, std::map;
using std::vector, std::initializer_list;


class ASTree {
public:
    enum TreeType {
        kIdentifier      = 1, 
        kNumber          = 2, 
        
        kEqual           = 3, 
        kNotEqual        = 4, 
        kGreater         = 5, 
        kLess            = 6, 
        kGreaterEqual    = 7, 
        kLessEqual       = 8,

        kAddition        = 9,
        kSubtraction     = 10, 
        kMultiplication  = 11, 
        kDivision        = 12, 
        kModulo          = 13,

        kDeclarations    = 14, 
        kParameters      = 15, 
        kNewProcedure    = 16, 
        kCallProcedure   = 17,

        kWrite           = 18, 
        kRead            = 19, 
        kCall            = 20, 
        kAssign          = 21,
        kRepeatLoop      = 22, 
        kWhileLoop       = 23,
        kIfCond          = 24, 
        kIfElseCond      = 25,

        kCommands        = 26,
        
        kMain            = 27, 
        kProcedures      = 28, 
        kProgram         = 29  
    };

    TreeType        mTreeType;
    int             mTreeMemoryIndex;
    int             mTreeBranchesCount;
    vector<ASTree *>  mTreeBranches;

    ASTree(TreeType type, int index, int branchesCount, initializer_list<ASTree *> subTrees);
};


void printPreorder(ASTree *tree, int depth);

ASTree startProgram(ASTree *procs, ASTree *main);

ASTree *newTreeValue(ASTree::TreeType type, int index);
ASTree *newTreeBranch(ASTree::TreeType type);
ASTree *newTreeBranch(ASTree::TreeType type, ASTree *arg);
ASTree *newTreeBranch(ASTree::TreeType type, ASTree *firstArg, ASTree *secondArg);

ASTree *addProcedure(ASTree *procs, ASTree *head, ASTree *comms);
ASTree *addProcedure(ASTree *procs, ASTree *head, ASTree *vars, ASTree *comms);
ASTree *addDeclaration(ASTree *head, ASTree *var);
ASTree *addCommand(ASTree *commands, ASTree *command);


#endif