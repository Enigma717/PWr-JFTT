all: bison flex
	g++ -W -pedantic --std=c++17 -o compiler main.cpp ./frontend/parser.cpp ./frontend/lexer.c

bison:
	bison -Wall -d -o ./frontend/parser.cpp ./frontend/parser.ypp

flex:
	flex -o ./frontend/lexer.c ./frontend/lexer.l

clean:
	rm ./frontend/parser.cpp ./frontend/parser.hpp ./frontend/lexer.c
