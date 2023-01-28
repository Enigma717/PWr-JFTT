/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Implementacja tablicy symboli
*/


#include <algorithm>

#include "./symtabnode.cpp"
#include "./symtab.h"


//////////////////////////////////////////////
// Tworzenie nowego węzła w tablicy symboli //
//////////////////////////////////////////////

int insertVarToSymTab(vector<SymTabNode> &symTab, string identifier)
{
    int memoryIndex = symTab.size();

    Variable var(identifier);
    SymTabNode node(var);

    node.mNodeIndex = memoryIndex;
    symTab.push_back(node);

    return memoryIndex;
}

int insertParToSymTab(vector<SymTabNode> &symTab, string identifier)
{
    int memoryIndex = symTab.size();

    Parameter par(identifier);
    SymTabNode node(par);

    node.mNodeIndex = memoryIndex;
    symTab.push_back(node);

    return memoryIndex;
}

int insertProcToSymTab(vector<SymTabNode> &symTab, string identifier)
{
    int memoryIndex = symTab.size();

    Procedure proc(identifier, memoryIndex);
    SymTabNode node(proc);

    symTab.push_back(node);

    return memoryIndex;
}

int insertNumToSymTab(vector<SymTabNode> &symTab, string numberString)
{
    int memoryIndex = symTab.size();

    SymTabNode node(numberString);

    node.mNodeIndex = memoryIndex;
    symTab.push_back(node);

    return memoryIndex;
}


//////////////////////////////////////
// Szukanie węzła w tablicy symboli //
//////////////////////////////////////

int seekNodeInSymTab(vector<SymTabNode> &symTab, string identifier)
{
    vector<SymTabNode>::iterator it = std::find_if(symTab.begin(), 
                                                   symTab.end(), 
                                                   [&](const SymTabNode &node) { 
                                                        return node.mNodeIdentifier == identifier; 
                                                   });
                               
    if (it != symTab.end())
    {
        return it->mNodeIndex;
    }
    
    return -1;
}

int seekNodeInSymTab(vector<SymTabNode> &symTab, string identifier, int scope)
{
    vector<SymTabNode>::iterator it = std::find_if(symTab.begin() + scope, 
                                                   symTab.end(), 
                                                   [&](const SymTabNode &node) { 
                                                        return node.mNodeIdentifier == identifier; 
                                                   });
    if (it != symTab.end())
    {
        return it->mNodeIndex;
    }
    
    return -1;
}
