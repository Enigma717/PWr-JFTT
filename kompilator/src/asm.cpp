/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Generowanie asemblera maszyny wirtualnej
*/


#include "./asm.h"


ASM(InstructionType instruction) : 
    mASMInstruction(ASMInstructionsStrings[instruction]) 
{
    mASMArgumentIndex = -1;
}


ASM(InstructionType instruction, int argument) :
    mASMInstruction(ASMInstructionsStrings(instruction)), mASMArgumentIndex(argument) {}

