/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Implementacja drzewa składniowego (header)
*/


#ifndef ASTREE_H
#define ASTREE_H


#include <vector>


using std::vector;


class ASTree {
public:
    enum TreeType {
        kIdentifier = 1, kNumber, 
        kEqual, kNotEqual, kGreater, kLess, kGreaterEqual, kLessEqual,
        kAddition, kSubtraction, kMultiplication, kDivision, kModulo,
        kDeclarations, kNewProcedure, kCallProcedure,
        kWrite, kRead, kCall, kAssign, 
        kRepeatLoop, kWhileLoop,
        kIfCond, kIfElseCond,
        kCommand,
        kMain, kProcedures, kProgram  
    };

    TreeType        mTreeType;
    int             mTreeMemoryIndex;
    int             mTreeBranchesCount;
    vector<ASTree>  mTreeBranches;
};


#endif