all: bison flex
	g++ -o compiler main.cpp ./frontend/parser.cpp ./frontend/lexer.c

bison:
	bison -d -o ./frontend/parser.cpp ./frontend/parser.ypp

flex:
	flex -o ./frontend/lexer.c ./frontend/lexer.l

clean:
	rm ./frontend/parser.cpp ./frontend/parser.hpp ./frontend/lexer.c