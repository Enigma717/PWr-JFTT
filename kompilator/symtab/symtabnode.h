/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Klasa węzłów tablicy symboli (header)
*/


#ifndef SYMTAB_NODE_H
#define SYMTAB_NODE_H


#include <string>

using std::string;


class Variable {
public:
    string          mVarIdentifier;
    long long int   mVarValue;

    Variable(string identifier);
};

class Parameter {
public:
    string          mParIdentifier;
    long long int   mParValue;

    Parameter(string identifier);
};

class Procedure {
public:
    string          mProcIdentifier;
    int             mProcMemoryIndex;

    Procedure(string identifier, int index);
};


class SymTabNode {
public:
    enum NodeType {
        kProcedure = 1, 
        kParameter = 2, 
        kVariable  = 3, 
        kNumber    = 4
    };

    NodeType        mNodeType;
    string          mNodeIdentifier;
    long long int   mNodeValue;
    int             mNodeIndex;
    int             mNodeParamCount;
    bool            mNodeIsInitialized;

    SymTabNode(Variable var);
    SymTabNode(Parameter par);
    SymTabNode(Procedure proc);
    SymTabNode(string numberString);
};


#endif