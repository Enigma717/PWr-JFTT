/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Generowanie asemblera maszyny wirtualnej
*/


#include <iostream>
#include <algorithm>

#include "./asm.h"

using std::cout;

ASM::ASM(InstructionType instruction) : 
    mASMInstruction(ASMInstructionsStrings[instruction]) 
{
    mASMArgumentIndex = -1;
}


ASM::ASM(InstructionType instruction, int argument) :
    mASMInstruction(ASMInstructionsStrings[instruction]), mASMArgumentIndex(argument) {}


////////////////////////////////////

void generateCode(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    if (tree == NULL)
    {
        return;
    }


    switch(tree->mTreeType) {
        // case ASTree::kIdentifier:
            // cout << symTab[tree->mTreeMemoryIndex].mNodeIdentifier
                //  << "\n";
//   
            // break;
        // case ASTree::kNumber:
// 
            // break;
        case ASTree::kEqual:

            break;

        case ASTree::kNotEqual:

            break;

        case ASTree::kGreater:

            break;

        case ASTree::kLess:

            break;

        case ASTree::kGreaterEqual:

            break;

        case ASTree::kLessEqual:

            break;
        
        case ASTree::kDeclarations:
            cout << "[BLOK DEKLARACJI]\n";

            break;

        case ASTree::kParameters:

            break;

        case ASTree::kNewProcedure:

            break;

        case ASTree::kCallProcedure:

            break;

        case ASTree::kWrite:
            cout << "\tPUT " << symTab[tree->mTreeBranches[0]->mTreeMemoryIndex].mNodeIndex << "\n";

            pushInstruction(code, ASM::kPut, symTab[tree->mTreeBranches[0]->mTreeMemoryIndex].mNodeIndex);

            break;

        case ASTree::kRead:
            cout << "\tGET " << symTab[tree->mTreeBranches[0]->mTreeMemoryIndex].mNodeIndex << "\n";

            pushInstruction(code, ASM::kGet, symTab[tree->mTreeBranches[0]->mTreeMemoryIndex].mNodeIndex);

            break;

        case ASTree::kCall:

            break;

        case ASTree::kAssign:
            cout << "[ASSIGN]:\n";
            addInstructionAssign(code, symTab, tree);

            break;

        case ASTree::kRepeatLoop:

            break;

        case ASTree::kWhileLoop:

            break;

        case ASTree::kIfCond:

            break;

        case ASTree::kIfElseCond:

            break;

        case ASTree::kCommands:
            cout << "[BLOK KOMEND]\n";

            break;

        case ASTree::kMain:
            cout << "[BLOK MAIN]\n";
            
            break;

        case ASTree::kProcedures:
            cout << "[BLOK PROCEDUR]\n";
            
            break;

        case ASTree::kProgram:
            cout << "\n\n[START PROGRAMU]\n"; 

            break;
    }
    
    
    for (ASTree *subTree : tree->mTreeBranches)
    {
        generateCode(code, symTab, subTree);
    }
}


////////////////////////////////////

void saveCodeToFile(std::ofstream &file, vector<ASM> &code)
{
    std::for_each(std::begin(code), std::end(code), [&file](ASM instruction) { 
        if (instruction.mASMArgumentIndex == -1) 
        {
            file << instruction.mASMInstruction << "\n";
        }
        else
        {
            file << instruction.mASMInstruction << " " 
                 << instruction.mASMArgumentIndex << "\n";
        }
    });
}


////////////////////////////////////

void pushInstruction(vector<ASM> &code, ASM::InstructionType type)
{
    code.push_back(ASM(type));
}

void pushInstruction(vector<ASM> &code, ASM::InstructionType type, int arg)
{
    code.push_back(ASM(type, arg));
}


////////////////////////////////////

void addConstants(vector<ASM> &code, vector<SymTabNode> &symTab)
{
    std::for_each(std::begin(symTab), std::end(symTab), [&code](SymTabNode node) { 
        if (node.mNodeType == SymTabNode::kNumber) 
        {
            cout << "\tSET " << node.mNodeValue << "\n";
            cout << "\tSTORE " << node.mNodeIndex << "\n";

            pushInstruction(code, ASM::kSet, node.mNodeValue);
            pushInstruction(code, ASM::kStore, node.mNodeIndex);
        }
    });
}

void addInstructionAssign(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    ASTree *var = tree->mTreeBranches[0];
    ASTree *exp = tree->mTreeBranches[1];

    switch (exp->mTreeType) {
        case ASTree::kIdentifier:
            cout << "\tLOAD " << symTab[exp->mTreeMemoryIndex].mNodeIndex << "\n";

            symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeMemoryIndex].mNodeValue;
                 
            pushInstruction(code, ASM::kLoad, symTab[exp->mTreeMemoryIndex].mNodeIndex);

            break;

        case ASTree::kNumber:
            cout << "\tLOAD " << symTab[exp->mTreeMemoryIndex].mNodeIndex << "\n";

            symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeMemoryIndex].mNodeValue;
                 
            pushInstruction(code, ASM::kLoad, symTab[exp->mTreeMemoryIndex].mNodeIndex);

            break;

        case ASTree::kAddition:
            cout << "\tLOAD " << symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeIndex << "\n";
            cout << "\tADD " << symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIndex;

            cout << "\t[ " << symTab[var->mTreeMemoryIndex].mNodeIdentifier << " := " 
                 << symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeIdentifier << " + " 
                 << symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIdentifier
                 << " ]\n";

            symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeValue + 
            symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeValue;

            pushInstruction(code, ASM::kLoad, symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeIndex);
            pushInstruction(code, ASM::kAdd, symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIndex);

            break;
        case ASTree::kSubtraction:

            break;
        case ASTree::kMultiplication:

            break;
        case ASTree::kDivision:

            break;
        case ASTree::kModulo:

            break;
    }

    cout << "\tSTORE " << symTab[var->mTreeMemoryIndex].mNodeIndex << "\n";

    pushInstruction(code, ASM::kStore, symTab[var->mTreeMemoryIndex].mNodeIndex);
}

