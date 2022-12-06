all: bison flex 
	gcc -o zad1 zad1bison.c zad1lex.c 
	rm -f zad1bison.* zad1lex.c

bison:
	bison -d -o zad1bison.c zad1.y

flex:
	flex -o zad1lex.c zad1.l