/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Implementacja węzłów tablicy symboli
*/


#include "./symtabnode.h"


////////////////////////////////
// Konstruktor klasy Variable //
////////////////////////////////

Variable::Variable(string identifier) : mVarIdentifier(identifier)
{
    mVarValue = -1;
}


/////////////////////////////////
// Konstruktor klasy Parameter //
/////////////////////////////////

Parameter::Parameter(string identifier) : mParIdentifier(identifier)
{
    mParValue = -1;
}

/////////////////////////////////
// Konstruktor klasy Procedure //
/////////////////////////////////

Procedure::Procedure(string identifier, int index) : 
    mProcIdentifier(identifier), mProcMemoryIndex(index) {}


///////////////////////////////////////
// Konstruktory klasy SymTabNode dla //
// każdego możliwego typu węzła      //
///////////////////////////////////////

SymTabNode::SymTabNode(Variable var) : 
    mNodeIdentifier(var.mVarIdentifier), mNodeValue(var.mVarValue) 
{
    mNodeType          = SymTabNode::kVariable;
    mNodeIndex         = -1;
    mNodeParamCount    = 0;
    mNodeIsInitialized = false;
}

SymTabNode::SymTabNode(Parameter par) : 
    mNodeIdentifier(par.mParIdentifier), mNodeValue(par.mParValue) 
{
    mNodeType          = SymTabNode::kParameter;
    mNodeIndex         = -1;
    mNodeParamCount    = 0;
    mNodeIsInitialized = false;
}

SymTabNode::SymTabNode(Procedure proc) :
    mNodeIdentifier(proc.mProcIdentifier), mNodeIndex(proc.mProcMemoryIndex) 
{
    mNodeType          = SymTabNode::kProcedure;
    mNodeValue         = -1;
    mNodeParamCount    = 0;
    mNodeIsInitialized = false;
}

SymTabNode::SymTabNode(string numberString) :
    mNodeIdentifier(numberString) 
{
    mNodeType          = SymTabNode::kNumber;
    mNodeValue         = stoll(numberString, nullptr, 10);
    mNodeIndex         = -1;
    mNodeParamCount    = 0;
    mNodeIsInitialized = false;
}