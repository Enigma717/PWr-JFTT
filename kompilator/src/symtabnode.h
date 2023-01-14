/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Klasy elementów tablicy symboli (header)
*/


#ifndef SYMTAB_NODE_H
#define SYMTAB_NODE_H


#include <string>


using std::string;


class Variable {
public:
    string          mVarIdentifier;
    long long int   mVarValue;

    Variable(string name);
};


class Procedure {
public:
    string          mProcIdentifier;
    int             mProcMemoryIndex;

    Procedure(string name, int index);
};


class SymTabNode {
public:
    enum NodeType {
        kProcedure = 1, 
        kVariable, 
        kNumber
    };

    NodeType        mNodeType;
    string          mNodeIdentifier;
    long long int   mNodeValue;
    int             mNodeIndex;
    int             mNodeParamCount;

    SymTabNode(Variable var);
    SymTabNode(Procedure proc);
    SymTabNode(string numberString);
};


#endif