/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Implementacja tablicy symboli (header)
*/


#ifndef SYMTAB_H
#define SYMTAB_H


#include <vector>
#include "./symtabnode.h"

using std::vector;


int insertProcToSymTab(vector<SymTabNode> &symTab, string identifier);
int insertVarToSymTab(vector<SymTabNode> &symTab, string identifier);
int insertNumToSymTab(vector<SymTabNode> &symTab, string numberString);

int seekNodeInSymTab(vector<SymTabNode> &symTab, string identifier);
int seekNodeInSymTab(vector<SymTabNode> &symTab, string identifier, int scope);


#endif