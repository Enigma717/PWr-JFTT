/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Klasa instrukcji asemblera maszyny wirtualnej (header)
*/


#ifndef ASM_H
#define ASM_H


#include "./symtab.h"


class ASM {
public:
    enum InstructionType {
        kGet     = 1,
        kPut     = 2,
        
        kLoad    = 3, 
        kStore   = 4, 
        kLoadI   = 5, 
        kStoreI  = 6, 
        
        kAdd     = 7, 
        kSub     = 8, 
        kAddI    = 9, 
        kSubI    = 10, 
        kSet     = 11, 
        kHalf    = 12, 
        
        kJump    = 13, 
        kJPos    = 14, 
        kJZero   = 15, 
        kJumpI   = 16, 
        
        kHalt    = 17 
    };

    string  mASMInstruction;
    int     mASMArgumentIndex;

    ASM(InstructionType instruction);
    ASM(InstructionType instruction, int argument);
};


map<ASM::InstructionType, std::string> ASMInstructionsStrings = {
    {ASM::kGet,     "GET"},
    {ASM::kPut,     "PUT"},
    {ASM::kLoad,    "LOAD"},
    {ASM::kStore,   "STORE"},
    {ASM::kLoadI,   "LOADI"},
    {ASM::kStoreI,  "STOREI"},
    {ASM::kAdd,     "ADD"},
    {ASM::kSub,     "SUB"},
    {ASM::kAddI,    "ADDI"},
    {ASM::kSubI,    "SUBI"},
    {ASM::kSet,     "SET"},
    {ASM::kHalf,    "HALF"},
    {ASM::kJump,    "JUMP"},
    {ASM::kJPos,    "JPOS"},
    {ASM::kJZero,   "JZERO"},
    {ASM::kJumpI,   "JUMPI"},
    {ASM::kHalt,    "HALT"},
};


void generateCode(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree);
void saveCodeToFile(std::ofstream &file, vector<ASM> &code);

void pushInstruction(vector<ASM> &code, ASM::InstructionType type);
void pushInstruction(vector<ASM> &code, ASM::InstructionType type, int arg);

void addConstants(vector<ASM> &code, vector<SymTabNode> &symTab);
void addInstructionAssign(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree);



#endif