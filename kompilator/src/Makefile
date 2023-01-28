CPPFLAGS = -W -pedantic -std=c++17

FRONT_DIR  = ./frontend
ASM_DIR    = ./asm
AST_DIR    = ./ast
SYMTAB_DIR = ./symtab


all: bison flex
	$(CXX) $(FLAGS) -o compiler main.cpp \
						$(FRONT_DIR)/parser.cpp $(FRONT_DIR)/lexer.c \
						$(ASM_DIR)/asm.cpp \
						$(AST_DIR)/astree.cpp \
						$(SYMTAB_DIR)/symtab.cpp $(SYMTAB_DIR)/symtabnode.cpp

flex: 
	flex -o $(FRONT_DIR)/lexer.c $(FRONT_DIR)/lexer.l

bison:
	bison -Wall -d -o $(FRONT_DIR)/parser.cpp $(FRONT_DIR)/parser.ypp

clean:
	rm $(FRONT_DIR)/parser.cpp $(FRONT_DIR)/parser.hpp $(FRONT_DIR)/lexer.c
