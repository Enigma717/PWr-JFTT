all: bison flex 
	gcc zad1.tab.c zad1lex.c -o zad1 -lm

bison:
	bison -d zad1.y

flex:
	flex -o zad1lex.c zad1.l
