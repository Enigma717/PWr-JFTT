/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Klasa zmiennej (header)
*/


#ifndef VARIABLE_H
#define VARIABLE_H


#include <string>

#include "./symtabnode.h"


class Variable : public SymTableNode {
public:
    std::string     mVarIdentifier;
    long long int   mVarValue;

    Variable(std::string name, long long int value) : 
        mVarIdentifier(name), mVarValue(value) {}
}


#endif