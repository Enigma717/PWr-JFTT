/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Klasa zmiennej (header)
*/


#ifndef VARIABLE_H
#define VARIABLE_H


#include <string>
#include <unordered_map>


class Variable {
public:
    std::string   mVarIdentifier;
    bool          mIsInitialized;
    long long int mVarMemoryIndex;
    Variable(name);
}




#endif