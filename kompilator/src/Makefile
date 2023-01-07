all: bison flex 
	g++ -o compiler main.cpp parser.cpp lexer.cpp

bison:
	bison -d -o parser.cpp parser.ypp

flex:
	flex -o lexer.cpp lexer.l