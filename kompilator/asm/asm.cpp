/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Generowanie asemblera maszyny wirtualnej
*/


#include <algorithm>
#include <fstream>

#include "./asm.h"


/* 
    Pomocniczne mapy/zmienne, niestety muszą być globalne
    przez rekurencję w generateCode() (pre-order walk po AST)
*/
map<string, long long int> procsOffsets;
map<string, long long int> procsStarts;

bool proceduresFlag = false;
int procOutOffset   = 1;
int asmLoopDepth    = 0;

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


////////////////////////////
// Konstruktory klasy ASM //
////////////////////////////

ASM::ASM(InstructionType instruction) : 
    mASMInstruction(ASMInstructionsStrings[instruction]) 
{
    mASMArgumentIndex = -1;
}


ASM::ASM(InstructionType instruction, long long int argument) :
    mASMInstruction(ASMInstructionsStrings[instruction]), mASMArgumentIndex(argument) {}


///////////////////////////////////////////////
// Generowanie kodu za pomocą pre-order walk //
///////////////////////////////////////////////

void generateCode(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree, const int mainStart)
{
    if (tree == NULL)
    {
        return;
    }

    bool skipComms = false;

    switch(tree->mTreeType) {
        case ASTree::kNewProcedure:
            proceduresFlag = true;
            skipComms      = true;

            createNewProcedure(code, symTab, tree);

            break;

        case ASTree::kCallProcedure:
            for (int i = 1; i <= symTab[tree->mTreeBranches[0]->mTreeMemoryIndex].mNodeParamCount; i++)
            {
                if (symTab[tree->mTreeBranches[1]->mTreeBranches[i - 1]->mTreeMemoryIndex].mNodeType == SymTabNode::kParameter)
                {
                    pushInstruction(code, ASM::kLoad, symTab[tree->mTreeBranches[1]->mTreeBranches[i - 1]->mTreeMemoryIndex].mNodeIndex);
                    pushInstruction(code, ASM::kStore, symTab[tree->mTreeBranches[0]->mTreeMemoryIndex + i].mNodeIndex);
                }
                else
                {
                    pushInstruction(code, ASM::kSet, symTab[tree->mTreeBranches[1]->mTreeBranches[i - 1]->mTreeMemoryIndex].mNodeIndex);
                    pushInstruction(code, ASM::kStore, symTab[tree->mTreeBranches[0]->mTreeMemoryIndex + i].mNodeIndex);
                }
            }
            
            pushInstruction(code, ASM::kSet, code.size() + 3);
            pushInstruction(code, ASM::kStore, procsOffsets[symTab[tree->mTreeBranches[0]->mTreeMemoryIndex].mNodeIdentifier]);

            pushInstruction(code, ASM::kJump, procsStarts[symTab[tree->mTreeBranches[0]->mTreeMemoryIndex].mNodeIdentifier]);

            break;

        case ASTree::kWrite:
            pushInstruction(code, ASM::kPut, symTab[tree->mTreeBranches[0]->mTreeMemoryIndex].mNodeIndex);

            break;

        case ASTree::kRead:
            pushInstruction(code, ASM::kGet, symTab[tree->mTreeBranches[0]->mTreeMemoryIndex].mNodeIndex);

            break;

        case ASTree::kAssign:
            addInstructionAssign(code, symTab, tree);

            break;

        case ASTree::kRepeatLoop:
            skipComms = true;
            
            addInstructionRepeat(code, symTab, tree);

            asmLoopDepth--;

            break;

        case ASTree::kWhileLoop:
            skipComms = true;

            addInstructionWhile(code, symTab, tree);

            asmLoopDepth--;

            break;

        case ASTree::kIfCond:
            skipComms = true;

            addInstructionIf(code, symTab, tree);

            break;

        case ASTree::kIfElseCond:
            skipComms = true;

            addInstructionIfElse(code, symTab, tree);

            break;

        case ASTree::kMain:
            if (proceduresFlag == true)
            {
                code[mainStart].mASMArgumentIndex = code.size();
            }
            else
            {
                code.erase(code.begin() + mainStart);
            }
            
            break;

        case ASTree::kProgram:
            pushInstruction(code, ASM::kJump);

            break;
    }
    
    
    if (skipComms != true)
    {
        for (ASTree *subTree : tree->mTreeBranches)
        {
            generateCode(code, symTab, subTree, mainStart);
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

void pushInstruction(vector<ASM> &code, ASM::InstructionType type, long long int arg)
{
    code.push_back(ASM(type, arg));
}


/////////////////////////////////
// Wpisanie stałych do pamięci //
///////////////////////////////// 

void addConstants(vector<ASM> &code, vector<SymTabNode> &symTab)
{
    std::for_each(std::begin(symTab), std::end(symTab), [&code](SymTabNode node) { 
        if (node.mNodeType == SymTabNode::kNumber) 
        {
            pushInstruction(code, ASM::kSet, node.mNodeValue);
            pushInstruction(code, ASM::kStore, node.mNodeIndex);
        }
    });
}


/////////////////////////////////////////
// Tworzenie indirect instrukcji jeśli //
// zmienna jest parametrem             //
/////////////////////////////////////////

void createStore(vector<ASM> &code, vector<SymTabNode> &symTab, int nodeIndex)
{
    if (symTab[nodeIndex].mNodeType == SymTabNode::kParameter)
    {
        pushInstruction(code, ASM::kStoreI, symTab[nodeIndex].mNodeIndex);
    }
    else
    {
        pushInstruction(code, ASM::kStore, symTab[nodeIndex].mNodeIndex);
    }
}

void createLoad(vector<ASM> &code, vector<SymTabNode> &symTab, int nodeIndex)
{
    if (symTab[nodeIndex].mNodeType == SymTabNode::kParameter)
    {
        pushInstruction(code, ASM::kLoadI, symTab[nodeIndex].mNodeIndex);
    }
    else
    {
        pushInstruction(code, ASM::kLoad, symTab[nodeIndex].mNodeIndex);
    }
}

void createAdd(vector<ASM> &code, vector<SymTabNode> &symTab, int nodeIndex)
{
    if (symTab[nodeIndex].mNodeType == SymTabNode::kParameter)
    {
        pushInstruction(code, ASM::kAddI, symTab[nodeIndex].mNodeIndex);
    }
    else
    {
        pushInstruction(code, ASM::kAdd, symTab[nodeIndex].mNodeIndex);
    }
}

void createSub(vector<ASM> &code, vector<SymTabNode> &symTab, int nodeIndex)
{
    if (symTab[nodeIndex].mNodeType == SymTabNode::kParameter)
    {
        pushInstruction(code, ASM::kSubI, symTab[nodeIndex].mNodeIndex);
    }
    else
    {
        pushInstruction(code, ASM::kSub, symTab[nodeIndex].mNodeIndex);
    }
}


///////////////////////////////
// Tworzenie nowej procedury //
///////////////////////////////

void createNewProcedure(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    long long int procOutIndex = symTab.size() + 3;
    long long int procStart    = code.size();

    string procId = symTab[tree->mTreeBranches[0]->mTreeMemoryIndex].mNodeIdentifier;

    if (tree->mTreeBranches.size() == 4)
    {
        generateCode(code, symTab, tree->mTreeBranches[3], 0);
    }
    else
    {
        generateCode(code, symTab, tree->mTreeBranches[2], 0);
    }

    procsOffsets.insert({procId, procOutIndex + procOutOffset});
    procsStarts.insert({procId, procStart});
    
    pushInstruction(code, ASM::kJumpI, procOutIndex + procOutOffset);

    procOutOffset++;
}


//////////////////////////////////
// Instrukcje warunkowe i pętle //
//////////////////////////////////

// Tworzenie warnuków

int createCondition(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    ASTree *cond           = tree->mTreeBranches[0];
    int condFirstArgIndex  = cond->mTreeBranches[0]->mTreeMemoryIndex;
    int condSecondArgIndex = cond->mTreeBranches[1]->mTreeMemoryIndex;

    int jumpIndex;


    switch (cond->mTreeType) {
        case ASTree::kEqual:
            createLoad(code, symTab, condFirstArgIndex);
            createSub(code, symTab, condSecondArgIndex);
            pushInstruction(code, ASM::kJPos);
            createLoad(code, symTab, condSecondArgIndex);
            createSub(code, symTab, condFirstArgIndex);

            jumpIndex = code.size();
            pushInstruction(code, ASM::kJPos);
            
            break;

        case ASTree::kNotEqual:
            createLoad(code, symTab, condFirstArgIndex);
            createSub(code, symTab, condSecondArgIndex);
            pushInstruction(code, ASM::kJPos);
            createLoad(code, symTab, condSecondArgIndex);
            createSub(code, symTab, condFirstArgIndex);
            pushInstruction(code, ASM::kJPos);

            jumpIndex = code.size();
            pushInstruction(code, ASM::kJump);
            
            break;

        case ASTree::kGreater:
            createLoad(code, symTab, condFirstArgIndex);
            createSub(code, symTab, condSecondArgIndex);

            jumpIndex = code.size();
            pushInstruction(code, ASM::kJZero);

            break;

        case ASTree::kLess:
            createLoad(code, symTab, condSecondArgIndex);
            createSub(code, symTab, condFirstArgIndex);

            jumpIndex = code.size();
            pushInstruction(code, ASM::kJZero);

            break;

        case ASTree::kGreaterEqual:
            createLoad(code, symTab, condFirstArgIndex);
            createSub(code, symTab, condSecondArgIndex);
            createLoad(code, symTab, condSecondArgIndex);
            createSub(code, symTab, condFirstArgIndex);

            jumpIndex = code.size();
            pushInstruction(code, ASM::kJPos);

            break;

        case ASTree::kLessEqual:
            createLoad(code, symTab, condSecondArgIndex);
            createSub(code, symTab, condFirstArgIndex);
            createLoad(code, symTab, condFirstArgIndex);
            createSub(code, symTab, condSecondArgIndex);

            jumpIndex = code.size();
            pushInstruction(code, ASM::kJPos);

            break;
    }

    return jumpIndex;
}

void createRepeatCondition(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree, int repeatStart)
{
    ASTree *cond           = tree->mTreeBranches[0];
    int condFirstArgIndex  = cond->mTreeBranches[0]->mTreeMemoryIndex;
    int condSecondArgIndex = cond->mTreeBranches[1]->mTreeMemoryIndex;


    switch (cond->mTreeType) {
        case ASTree::kEqual:
            createLoad(code, symTab, condFirstArgIndex);
            createSub(code, symTab, condSecondArgIndex);
            pushInstruction(code, ASM::kJPos, repeatStart);
            createLoad(code, symTab, condSecondArgIndex);
            createSub(code, symTab, condFirstArgIndex);
            pushInstruction(code, ASM::kJPos, repeatStart);
            
            break;

        case ASTree::kNotEqual:
            createLoad(code, symTab, condFirstArgIndex);
            createSub(code, symTab, condSecondArgIndex);
            pushInstruction(code, ASM::kJPos, static_cast<long long int>(code.size() + 5));
            createLoad(code, symTab, condSecondArgIndex);
            createSub(code, symTab, condFirstArgIndex);
            pushInstruction(code, ASM::kJPos, static_cast<long long int>(code.size() + 2));
            pushInstruction(code, ASM::kJZero, repeatStart);

            break;

        case ASTree::kGreater:
            createLoad(code, symTab, condFirstArgIndex);
            createSub(code, symTab, condSecondArgIndex);
            pushInstruction(code, ASM::kJZero, repeatStart);

            break;

        case ASTree::kLess:
            createLoad(code, symTab, condSecondArgIndex);
            createSub(code, symTab, condFirstArgIndex);
            pushInstruction(code, ASM::kJZero, repeatStart);

            break;

        case ASTree::kGreaterEqual:
            createLoad(code, symTab, condFirstArgIndex);
            createSub(code, symTab, condSecondArgIndex);
            createLoad(code, symTab, condSecondArgIndex);
            createSub(code, symTab, condFirstArgIndex);
            pushInstruction(code, ASM::kJPos, repeatStart);

            break;

        case ASTree::kLessEqual:
            createLoad(code, symTab, condSecondArgIndex);
            createSub(code, symTab, condFirstArgIndex);
            createLoad(code, symTab, condFirstArgIndex);
            createSub(code, symTab, condSecondArgIndex);
            pushInstruction(code, ASM::kJPos, repeatStart);

            break;
    }
}

void fixConditionJump(vector<ASM> &code, ASTree::TreeType type, int index)
{
    int jumpArg = code.size();

    switch (type) {
        case ASTree::kEqual:
            code[index].mASMArgumentIndex     = jumpArg;
            code[index - 3].mASMArgumentIndex = jumpArg;
            
            break;

        case ASTree::kNotEqual:
            code[index].mASMArgumentIndex     = jumpArg;
            code[index - 1].mASMArgumentIndex = index + 1;
            code[index - 4].mASMArgumentIndex = index + 1;

            break;

        case ASTree::kGreater:
            code[index].mASMArgumentIndex = jumpArg;

            break;

        case ASTree::kLess:
            code[index].mASMArgumentIndex = jumpArg;

            break;

        case ASTree::kGreaterEqual:
            code[index].mASMArgumentIndex = jumpArg;

            break;

        case ASTree::kLessEqual:
            code[index].mASMArgumentIndex = jumpArg;

            break;
    }
}


// Generowanie instrukcji warunkowych i pętli
// wraz z ich komendami

void addInstructionIf(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    ASTree::TreeType condType = tree->mTreeBranches[0]->mTreeType;
    ASTree *commands          = tree->mTreeBranches[1];

    int jumpIndex = createCondition(code, symTab, tree);
    generateCode(code, symTab, commands, 0);

    fixConditionJump(code, condType, jumpIndex);
}

void addInstructionIfElse(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    ASTree::TreeType condType = tree->mTreeBranches[0]->mTreeType;
    ASTree *commandsIf        = tree->mTreeBranches[1];
    ASTree *commandsElse      = tree->mTreeBranches[2];

    int jumpToElseIndex = createCondition(code, symTab, tree);
    generateCode(code, symTab, commandsIf, 0);

    int jumpToEndIndex = code.size();
    pushInstruction(code, ASM::kJump);

    fixConditionJump(code, condType, jumpToElseIndex);
    generateCode(code, symTab, commandsElse, 0);

    code[jumpToEndIndex].mASMArgumentIndex = code.size();
}

void addInstructionWhile(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    ASTree::TreeType condType = tree->mTreeBranches[0]->mTreeType;
    ASTree *commands          = tree->mTreeBranches[1];

    int jumpArg = code.size();
    int jumpIndex = createCondition(code, symTab, tree);
    asmLoopDepth++;

    generateCode(code, symTab, commands, 0);
    pushInstruction(code, ASM::kJump, jumpArg);

    fixConditionJump(code, condType, jumpIndex);
}

void addInstructionRepeat(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    ASTree *commands = tree->mTreeBranches[1];

    int jumpArg = code.size();
    asmLoopDepth++;
    
    generateCode(code, symTab, commands, 0);

    createRepeatCondition(code, symTab, tree, jumpArg);
}


/////////////////////////////////
// Przypisanie oraz arytmetyka //
/////////////////////////////////

void addInstructionAssign(vector<ASM> &code, vector<SymTabNode> &symTab, ASTree *tree)
{
    ASTree *var = tree->mTreeBranches[0];
    ASTree *exp = tree->mTreeBranches[1];

    int auxVarIndex = symTab.size();


    switch (exp->mTreeType) {
        case ASTree::kIdentifier:
            symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeMemoryIndex].mNodeValue;

            createLoad(code, symTab, exp->mTreeMemoryIndex);

            break;

        case ASTree::kNumber:
            symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeMemoryIndex].mNodeValue;

            pushInstruction(code, ASM::kLoad, symTab[exp->mTreeMemoryIndex].mNodeIndex);

            break;

        case ASTree::kAddition:
            symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeValue + 
                                                       symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeValue;

            createLoad(code, symTab, exp->mTreeBranches[0]->mTreeMemoryIndex);
            createAdd(code, symTab, exp->mTreeBranches[1]->mTreeMemoryIndex);

            break;

        case ASTree::kSubtraction:
            symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeValue - 
                                                       symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeValue;

            createLoad(code, symTab, exp->mTreeBranches[0]->mTreeMemoryIndex);
            createSub(code, symTab, exp->mTreeBranches[1]->mTreeMemoryIndex);
            
            break;

        case ASTree::kMultiplication:
            symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeValue * 
                                                       symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeValue;

            if (symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIdentifier == "2")
            {
                createLoad(code, symTab, exp->mTreeBranches[0]->mTreeMemoryIndex);
                pushInstruction(code, ASM::kAdd, 0);
            }
            else if (symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeIdentifier == "2")
            {
                createLoad(code, symTab, exp->mTreeBranches[1]->mTreeMemoryIndex);
                pushInstruction(code, ASM::kAdd, 0);
            }
            else
            {
                //////////////////////////////
                // auxVarIndex:      result //
                // auxVarIndex + 1:   xcopy //
                // auxVarIndex + 2:   ycopy //
                // auxVarIndex + 3:    temp //
                //////////////////////////////


                // result = 0
                pushInstruction(code, ASM::kSet, 0);
                pushInstruction(code, ASM::kStore, auxVarIndex);
                // xcopy = x
                createLoad(code, symTab, exp->mTreeBranches[0]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kStore, auxVarIndex + 1);
                // ycopy = y
                createLoad(code, symTab, exp->mTreeBranches[1]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kStore, auxVarIndex + 2);


                // while ycopy > 0
                // createLoad(code, symTab, auxVarIndex + 2); 
                pushInstruction(code, ASM::kLoad, auxVarIndex + 2);
                pushInstruction(code, ASM::kJZero, static_cast<long long int>(code.size() + 17));

                // temp = (ycopy / 2) * 2
                pushInstruction(code, ASM::kHalf);
                pushInstruction(code, ASM::kAdd, 0);
                pushInstruction(code, ASM::kStore, auxVarIndex + 3);


                // if (ycopy - temp) == 0
                pushInstruction(code, ASM::kLoad, auxVarIndex + 2);
                pushInstruction(code, ASM::kSub, auxVarIndex + 3);
                pushInstruction(code, ASM::kJZero, static_cast<long long int>(code.size() + 4));
            
                // result = result + xcopy 
                pushInstruction(code, ASM::kLoad, auxVarIndex);
                pushInstruction(code, ASM::kAdd, auxVarIndex + 1);
                pushInstruction(code, ASM::kStore, auxVarIndex);
                // koniec ifa
                

                // xcopy = xcopy * 2
                pushInstruction(code, ASM::kLoad, auxVarIndex + 1);
                pushInstruction(code, ASM::kAdd, 0);
                pushInstruction(code, ASM::kStore, auxVarIndex + 1);

                // ycopy = ycopy / 2
                pushInstruction(code, ASM::kLoad, auxVarIndex + 2);
                pushInstruction(code, ASM::kHalf);
                pushInstruction(code, ASM::kStore, auxVarIndex + 2);

                pushInstruction(code, ASM::kJump, static_cast<long long int>(code.size() - 17));
                // koniec while


                // return result
                pushInstruction(code, ASM::kLoad, auxVarIndex);
            }

            break;

        case ASTree::kDivision:
            if (symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeValue == 0)
            {
                symTab[var->mTreeMemoryIndex].mNodeValue = 0; 
            }
            else
            {
                symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeValue / 
                                                           symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeValue;
            }


            if (symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIdentifier == "0")
            {
                pushInstruction(code, ASM::kSet, 0);
            }
            else if (symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIdentifier == "2")
            {
                createLoad(code, symTab, exp->mTreeBranches[0]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kHalf);
            }
            else if (symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeValue == 2 && asmLoopDepth == 0)
            {
                createLoad(code, symTab, exp->mTreeBranches[0]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kHalf);
            }
            else
            {
                ///////////////////////////////
                // auxVarIndex:       result //
                // auxVarIndex + 1:   modulo //
                // auxVarIndex + 2:  divisor //
                // auxVarIndex + 3:   binary //
                ///////////////////////////////


                // result = 0 && modulo = 0
                pushInstruction(code, ASM::kSet, 0);
                pushInstruction(code, ASM::kStore, auxVarIndex);
                pushInstruction(code, ASM::kStore, auxVarIndex + 1);


                // if y == 0
                createLoad(code, symTab, exp->mTreeBranches[1]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kJZero, static_cast<long long int>(code.size() + 38));

                // modulo = x
                createLoad(code, symTab, exp->mTreeBranches[0]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kStore, auxVarIndex + 1);
                

                // while1 modulo >= y
                pushInstruction(code, ASM::kLoad, auxVarIndex + 1);
                createSub(code, symTab, exp->mTreeBranches[1]->mTreeMemoryIndex); 
                createLoad(code, symTab, exp->mTreeBranches[1]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kSub, auxVarIndex + 1);
                pushInstruction(code, ASM::kJPos, static_cast<long long int>(code.size() + 30));

                // divisor = y
                createLoad(code, symTab, exp->mTreeBranches[1]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kStore, auxVarIndex + 2);
                // binary = 1
                pushInstruction(code, ASM::kSet, 1);
                pushInstruction(code, ASM::kStore, auxVarIndex + 3);


                // while2 divisor <= modulo
                pushInstruction(code, ASM::kLoad, auxVarIndex + 2);
                pushInstruction(code, ASM::kSub, auxVarIndex + 1);
                pushInstruction(code, ASM::kLoad, auxVarIndex + 2);
                pushInstruction(code, ASM::kSub, auxVarIndex + 1);
                pushInstruction(code, ASM::kJPos, static_cast<long long int>(code.size() + 8));

                // divisor = divisor * 2
                pushInstruction(code, ASM::kLoad, auxVarIndex + 2);
                pushInstruction(code, ASM::kAdd, 0);
                pushInstruction(code, ASM::kStore, auxVarIndex + 2);
                // binary = binary * 2
                pushInstruction(code, ASM::kLoad, auxVarIndex + 3);
                pushInstruction(code, ASM::kAdd, 0);
                pushInstruction(code, ASM::kStore, auxVarIndex + 3);
                
                pushInstruction(code, ASM::kJump, static_cast<long long int>(code.size() - 11));
                // koniec while2

                
                // divisor = divisor / 2
                pushInstruction(code, ASM::kLoad, auxVarIndex + 2);
                pushInstruction(code, ASM::kHalf);
                pushInstruction(code, ASM::kStore, auxVarIndex + 2);
                // binary = binary / 2
                pushInstruction(code, ASM::kLoad, auxVarIndex + 3);
                pushInstruction(code, ASM::kHalf);
                pushInstruction(code, ASM::kStore, auxVarIndex + 3);

                
                // result = result + binary
                pushInstruction(code, ASM::kLoad, auxVarIndex);
                pushInstruction(code, ASM::kAdd, auxVarIndex + 3);
                pushInstruction(code, ASM::kStore, auxVarIndex);
                // modulo = modulo - divisor
                pushInstruction(code, ASM::kLoad, auxVarIndex + 1);
                pushInstruction(code, ASM::kSub, auxVarIndex + 2);
                pushInstruction(code, ASM::kStore, auxVarIndex + 1);

                pushInstruction(code, ASM::kJump, static_cast<long long int>(code.size() - 33));
                // koniec while1
               

                // return result
                pushInstruction(code, ASM::kLoad, auxVarIndex);
            }

            break;

        case ASTree::kModulo:
            if (symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeValue == 0)
            {
                symTab[var->mTreeMemoryIndex].mNodeValue = 0; 
            }
            else
            {
                symTab[var->mTreeMemoryIndex].mNodeValue = symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeValue % 
                                                           symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeValue;
            }


            if (symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIdentifier == "0")
            {
                pushInstruction(code, ASM::kSet, 0);
            }
            else if (symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeIdentifier == "2")
            {
                createLoad(code, symTab, exp->mTreeBranches[0]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kHalf);
                pushInstruction(code, ASM::kAdd, 0);
                pushInstruction(code, ASM::kStore, auxVarIndex);

                pushInstruction(code, ASM::kLoad, symTab[exp->mTreeBranches[0]->mTreeMemoryIndex].mNodeIndex);
                pushInstruction(code, ASM::kSub, auxVarIndex);
            }
            else if (symTab[exp->mTreeBranches[1]->mTreeMemoryIndex].mNodeValue == 2 && asmLoopDepth == 0)
            {
                createLoad(code, symTab, exp->mTreeBranches[0]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kHalf);
                pushInstruction(code, ASM::kAdd, 0);
                pushInstruction(code, ASM::kStore, auxVarIndex);

                createLoad(code, symTab, exp->mTreeBranches[0]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kSub, auxVarIndex);
            }
            else
            {
                ///////////////////////////////
                // auxVarIndex:       result //
                // auxVarIndex + 1:   modulo //
                // auxVarIndex + 2:  divisor //
                // auxVarIndex + 3:   binary //
                ///////////////////////////////


                // result = 0 && modulo = 0
                pushInstruction(code, ASM::kSet, 0);
                pushInstruction(code, ASM::kStore, auxVarIndex);
                pushInstruction(code, ASM::kStore, auxVarIndex + 1);


                // if y == 0
                createLoad(code, symTab, exp->mTreeBranches[1]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kJZero, static_cast<long long int>(code.size() + 29));

                // modulo = x
                createLoad(code, symTab, exp->mTreeBranches[0]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kStore, auxVarIndex + 1);
                

                // while1 modulo >= y
                pushInstruction(code, ASM::kLoad, auxVarIndex + 1);
                createSub(code, symTab, exp->mTreeBranches[1]->mTreeMemoryIndex); 
                createLoad(code, symTab, exp->mTreeBranches[1]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kSub, auxVarIndex + 1);
                pushInstruction(code, ASM::kJPos, static_cast<long long int>(code.size() + 21));

                // divisor = y
                createLoad(code, symTab, exp->mTreeBranches[1]->mTreeMemoryIndex); 
                pushInstruction(code, ASM::kStore, auxVarIndex + 2);
                // binary = 1
                pushInstruction(code, ASM::kSet, 1);
                pushInstruction(code, ASM::kStore, auxVarIndex + 3);


                // while2 divisor <= modulo
                pushInstruction(code, ASM::kLoad, auxVarIndex + 2);
                pushInstruction(code, ASM::kSub, auxVarIndex + 1);
                pushInstruction(code, ASM::kLoad, auxVarIndex + 2);
                pushInstruction(code, ASM::kSub, auxVarIndex + 1);
                pushInstruction(code, ASM::kJPos, static_cast<long long int>(code.size() + 5));

                // divisor = divisor * 2
                pushInstruction(code, ASM::kLoad, auxVarIndex + 2);
                pushInstruction(code, ASM::kAdd, 0);
                pushInstruction(code, ASM::kStore, auxVarIndex + 2);

                pushInstruction(code, ASM::kJump, static_cast<long long int>(code.size() - 8));
                // koniec while2

                
                // divisor = divisor / 2
                pushInstruction(code, ASM::kLoad, auxVarIndex + 2);
                pushInstruction(code, ASM::kHalf);
                pushInstruction(code, ASM::kStore, auxVarIndex + 2);


                // modulo = modulo - divisor
                pushInstruction(code, ASM::kLoad, auxVarIndex + 1);
                pushInstruction(code, ASM::kSub, auxVarIndex + 2);
                pushInstruction(code, ASM::kStore, auxVarIndex + 1);

                pushInstruction(code, ASM::kJump, static_cast<long long int>(code.size() - 24));
                // koniec while1
               

                // return result
                pushInstruction(code, ASM::kLoad, auxVarIndex + 1);
            }

            break;
    }

    createStore(code, symTab, var->mTreeMemoryIndex);
}

