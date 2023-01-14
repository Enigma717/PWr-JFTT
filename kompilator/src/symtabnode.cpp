/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Klasy elementów tablicy symboli
*/


#include "./symtabnode.h"


Variable::Variable(string name) : mVarIdentifier(name)
{
    mVarValue = -1;
}


Procedure::Procedure(string name, int index) : 
    mProcIdentifier(name), mProcMemoryIndex(index) {}


SymTabNode::SymTabNode(Procedure proc) :
    mNodeIdentifier(proc.mProcIdentifier), mNodeIndex(proc.mProcMemoryIndex) 
{
    mNodeType = SymTabNode::kProcedure;
    mNodeValue = -1;
    mNodeParamCount = 0;
}

SymTabNode::SymTabNode(Variable var) : 
    mNodeIdentifier(var.mVarIdentifier), mNodeValue(var.mVarValue) 
{
    mNodeType = SymTabNode::kVariable;
    mNodeParamCount = 0;
}

SymTabNode::SymTabNode(string numberString) :
    mNodeIdentifier(numberString) 
{
    mNodeType = SymTabNode::kNumber;
    mNodeValue = stoll(numberString, nullptr, 10);
    mNodeParamCount = 0;
}