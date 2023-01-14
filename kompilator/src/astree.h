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
        kIdentifier = 1, 
        kNumber = 2, 
        
        kEqual = 3, 
        kNotEqual = 4, 
        kGreater = 5, 
        kLess = 6, 
        kGreaterEqual = 7, 
        kLessEqual = 8,

        kAddition = 9,
        kSubtraction = 10, 
        kMultiplication = 11, 
        kDivision = 12, 
        kModulo = 13,

        kDeclarations = 14, 
        kNewProcedure = 15, 
        kCallProcedure = 16,

        kWrite = 17, 
        kRead = 18, 
        kCall = 19, 
        kAssign = 20,

        kRepeatLoop = 21, 
        kWhileLoop = 22,
        
        kIfCond = 23, 
        kIfElseCond = 24,

        kCommand = 25,
        
        kMain = 26, 
        kProcedures = 27, 
        kProgram = 28  
    };

    TreeType        mTreeType;
    int             mTreeMemoryIndex;
    int             mTreeBranchesCount;
    vector<ASTree *>  mTreeBranches;

    ASTree(TreeType type, int index, int branchesCount, initializer_list<ASTree *> subTrees);
};



ASTree *newTreeValue(ASTree::TreeType type, int index);
ASTree *newTreeStatement(ASTree::TreeType type, ASTree *firstArg, ASTree *secondArg);
ASTree *newTreeAssign(ASTree::TreeType type, ASTree *leftSide, ASTree *rightSide);



#endif