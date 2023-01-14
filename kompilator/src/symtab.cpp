/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Implementacja tablicy symboli
*/


#include <algorithm>

#include "./symtabnode.h"
#include "./symtab.h"


int insertProcToSymTab(string identifier)
{
    int memoryIndex = symbolTable.size();

    Procedure proc(identifier, memoryIndex);
    SymTabNode node(proc);

    symbolTable.push_back(node);

    return memoryIndex;
}

int insertVarToSymTab(string identifier)
{
    int memoryIndex = symbolTable.size();

    Variable var(identifier);
    SymTabNode node(var);

    node.mNodeIndex = memoryIndex;
    symbolTable.push_back(node);

    return memoryIndex;
}

int insertNumToSymTab(string numberString)
{
    int memoryIndex = symbolTable.size();

    SymTabNode node(numberString);

    node.mNodeIndex = memoryIndex;
    symbolTable.push_back(node);

    return memoryIndex;
}


int seekNodeInSymTab(string identifier)
{
    vector<SymTabNode>::iterator it = std::find_if(symbolTable.begin(), 
                                                   symbolTable.end(), 
                                                   [&](const SymTabNode &node) { 
                                                        return node.mNodeIdentifier == identifier; 
                                                   });
                               
    if (it != symbolTable.end())
    {
        return it->mNodeIndex;
    }
    
    return -1;
}

int seekNodeInSymTab(string identifier, int scope)
{
    vector<SymTabNode>::iterator it = std::find_if(symbolTable.begin() + scope, 
                                                   symbolTable.end(), 
                                                   [&](const SymTabNode &node) { 
                                                        return node.mNodeIdentifier == identifier; 
                                                   });
    if (it != symbolTable.end())
    {
        return it->mNodeIndex;
    }
    
    return -1;
}
