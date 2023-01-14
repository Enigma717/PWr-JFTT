/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Implementacja tablicy symboli (header)
*/


#ifndef SYMTAB_H
#define SYMTAB_H


#include <vector>


using std::vector;


vector<SymTabNode> symbolTable;


int insertProcToSymTab(string identifier);
int insertVarToSymTab(string identifier);
int insertNumToSymTab(string numberString);

int seekNodeInSymTab(string identifier);
int seekNodeInSymTab(string identifier, int scope);



#endif