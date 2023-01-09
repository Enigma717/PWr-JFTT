/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Klasa procedury (header)
*/


#ifndef PROCEDURE_H
#define PROCEDURE_H


#include <string>


class Procedure {
public:
    std::string   mProcIdentifier;
    long long int mProcMemoryIndex;
    bool          mIsInitialized;

    unordered_map<std::string, Variable> mLocalVariables;


    Procedure(std::string procId, long long int procMemIndex, bool initFlag);
}




#endif
