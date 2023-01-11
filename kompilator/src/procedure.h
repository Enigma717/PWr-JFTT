/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Klasa procedury (header)
*/


#ifndef PROCEDURE_H
#define PROCEDURE_H


#include <string>
#include <unordered_map>

#include "./variable.h"


class Procedure : public SymTableNode {
public:
    std::string                             mProcIdentifier;
    unordered_map<std::string, Variable>    mLocalVariables;


    Procedure(std::string name) : mProcIdentifier(name) {}
    Procedure(std::string name, unordered_map<std::string, Variable> vars) :
        mProcIdentifier(name), mLocalVariables(vars) {}
}




#endif
