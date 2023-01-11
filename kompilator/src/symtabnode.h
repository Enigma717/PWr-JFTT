/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Klasa elementu w tablicy symboli (header)
*/


#ifndef SYMTABLENODE_H
#define SYMTABLENODE_H


class SymTableNode {
public:
    int mNodeMemIndex;

    virtual ~SymTableNode() = 0 {}
}


#endif
