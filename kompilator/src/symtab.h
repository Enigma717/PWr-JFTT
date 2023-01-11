/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Klasa tablicy symboli (header)
*/


#ifndef SYMTABLE_H
#define SYMTABLE_H


#include <string>
#include <unordered_map>

#include "./procedure.h"


class SymTable {
public:
    unordered_map<std::string, SymTableNode> symbolTable; 

    SymTable() {}
}




#endif
