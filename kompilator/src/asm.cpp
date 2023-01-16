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


//////////////////////
// Konstruktory ASM //
//////////////////////

ASM::ASM(InstructionType instruction) : 
    mASMInstruction(ASMInstructionsStrings[instruction]) 
{
    mASMArgumentIndex = -1;
}


ASM::ASM(InstructionType instruction, int argument) :
    mASMInstruction(ASMInstructionsStrings[instruction]), mASMArgumentIndex(argument) {}


///////////////////////////////////////////////
// Generowanie kodu za pomocą pre-order walk //
///////////////////////////////////////////////

void generateCode(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    if (tree == NULL)
    {
        return;
    }

    bool skipComms = false;


    switch(tree->mTreeType) {
        // case ASTree::kIdentifier:
        //     if (symTab[tree->mTreeMemoryIndex].mNodeType == SymTabNode::kProcedure)
        //     {
        //         cout << "[PROCEDURA " << symTab[tree->mTreeMemoryIndex].mNodeIdentifier << "]\n";
        //     }

        //     break;
        // case ASTree::kNumber:
// 
            // break;
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
            cout << "[ASSIGN]\n";
            addInstructionAssign(code, symTab, tree);

            break;

        case ASTree::kRepeatLoop:
            cout << "[REPEAT]\n";

            addInstructionRepeat(code, symTab, tree);
            skipComms = true;

            break;

        case ASTree::kWhileLoop:
            cout << "[WHILE]\n";

            addInstructionWhile(code, symTab, tree);
            skipComms = true;

            break;

        case ASTree::kIfCond:
            cout << "[IF]\n";

            addInstructionIf(code, symTab, tree);
            skipComms = true;

            break;

        case ASTree::kIfElseCond:
            cout << "[IFELSE]\n";

            addInstructionIfElse(code, symTab, tree);
            skipComms = true;

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
    
    
    if (skipComms != true)
    {
        for (ASTree *subTree : tree->mTreeBranches)
        {
            generateCode(code, symTab, subTree);
        }
    }
}


///////////////////////////////////////////
// Zapisanie wektora instrukcji do pliku //
///////////////////////////////////////////

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


/////////////////////////////////////////////
// Dopisywanie nowej instrukcji do wektora //
/////////////////////////////////////////////

void pushInstruction(vector<ASM> &code, ASM::InstructionType type)
{
    code.push_back(ASM(type));
}

void pushInstruction(vector<ASM> &code, ASM::InstructionType type, int arg)
{
    code.push_back(ASM(type, arg));
}


////////////////////////////////////////
// Tworzenie warunków do ifów i pętli //
////////////////////////////////////////

int createCondition(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    ASTree *cond = tree->mTreeBranches[0];

    int condFirstArgIndex = cond->mTreeBranches[0]->mTreeMemoryIndex;
    int condSecondArgIndex = cond->mTreeBranches[1]->mTreeMemoryIndex;

    int jumpIndex;


    switch (cond->mTreeType) {
        case ASTree::kEqual:

            break;

        case ASTree::kNotEqual:

            break;

        case ASTree::kGreater:
            // do usunięcia
            cout << "\tLOAD " << symTab[condFirstArgIndex].mNodeIndex << "\n";
            cout << "\tSUB " << symTab[condSecondArgIndex].mNodeIndex;

            cout << "\t[ " 
                 << symTab[condFirstArgIndex].mNodeIdentifier << " > " << symTab[condSecondArgIndex].mNodeIdentifier
                 << " ]\n";
            // do usunięcia

            pushInstruction(code, ASM::kLoad, symTab[condFirstArgIndex].mNodeIndex);
            pushInstruction(code, ASM::kSub, symTab[condSecondArgIndex].mNodeIndex);

            jumpIndex = code.size();

            pushInstruction(code, ASM::kJZero);

            cout << "\tJZERO " << jumpIndex << "\n";

            break;

        case ASTree::kLess:

            break;

        case ASTree::kGreaterEqual:

            break;

        case ASTree::kLessEqual:

            break;
    }

    return jumpIndex;
}


////////////////////////////////////////////////////
// Tworzenie bloków instrukcji ze słów kluczowych //
////////////////////////////////////////////////////

        /*    Zapisanie stałych w pamięci    */ 

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


        /*    Assign z wewnętrznym poddrzewem expression    */  
        
void addInstructionAssign(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    ASTree *var = tree->mTreeBranches[0];
    ASTree *exp = tree->mTreeBranches[1];

    switch (exp->mTreeType) {
        case ASTree::kIdentifier:
            // do usunięcia
            cout << "\tLOAD " << symTab[exp->mTreeMemoryIndex].mNodeIndex << "\n";

            symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeMemoryIndex].mNodeValue;
            // do usunięcia
                 
            pushInstruction(code, ASM::kLoad, symTab[exp->mTreeMemoryIndex].mNodeIndex);

            break;

        case ASTree::kNumber:
            // do usunięcia
            cout << "\tLOAD " << symTab[exp->mTreeMemoryIndex].mNodeIndex << "\n";

            symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeMemoryIndex].mNodeValue;
            // do usunięcia
                 
            pushInstruction(code, ASM::kLoad, symTab[exp->mTreeMemoryIndex].mNodeIndex);

            break;

        case ASTree::kAddition:
            // do usunięcia
            cout << "\tLOAD " << symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeIndex << "\n";
            cout << "\tADD " << symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIndex;

            cout << "\t[ " << symTab[var->mTreeMemoryIndex].mNodeIdentifier << " := " 
                 << symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeIdentifier << " + " 
                 << symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIdentifier
                 << " ]\n";

            symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeValue + 
            symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeValue;
            // do usunięcia

            pushInstruction(code, ASM::kLoad, symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeIndex);
            pushInstruction(code, ASM::kAdd, symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIndex);

            break;

        case ASTree::kSubtraction:
            // do usunięcia
            cout << "\tLOAD " << symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeIndex << "\n";
            cout << "\tSUB " << symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIndex;

            cout << "\t[ " << symTab[var->mTreeMemoryIndex].mNodeIdentifier << " := " 
                 << symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeIdentifier << " - " 
                 << symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIdentifier
                 << " ]\n";

            symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeValue - 
            symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeValue;
            // do usunięcia

            pushInstruction(code, ASM::kLoad, symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeIndex);
            pushInstruction(code, ASM::kSub, symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIndex);
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


        /*    Instrukcje warunkowe i pętle    */

void addInstructionIf(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    int jumpIndex = createCondition(code, symTab, tree);
    generateCode(code, symTab, tree->mTreeBranches[1]);

    code[jumpIndex].mASMArgumentIndex = code.size();
}

void addInstructionIfElse(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    int jumpToElseIndex = createCondition(code, symTab, tree);
    generateCode(code, symTab, tree->mTreeBranches[1]);

    int jumpToEndIndex = code.size();
    pushInstruction(code, ASM::kJump);

    code[jumpToElseIndex].mASMArgumentIndex = code.size();
    generateCode(code, symTab, tree->mTreeBranches[2]);

    code[jumpToEndIndex].mASMArgumentIndex = code.size();
}

void addInstructionWhile(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    int jumpArg = code.size();

    int jumpIndex = createCondition(code, symTab, tree);
    generateCode(code, symTab, tree->mTreeBranches[1]);

    pushInstruction(code, ASM::kJump, jumpArg);

    code[jumpIndex].mASMArgumentIndex = code.size();
}

void addInstructionRepeat(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    // int jumpArg = code.size();
    // generateCode(code, symTab, tree->mTreeBranches[1]);
// 
    // int jumpIndex = createConditionRepeat(code, symTab, tree);
    // code[jumpIndex].mASMArgumentIndex = jumpArg;
}
