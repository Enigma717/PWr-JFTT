# !/bin/bash

lex -o zad1.c zad1.l
gcc zad1.c -o zad1
./zad1 < test1.txt > lexed1.txt
./zad1 < test2.txt > lexed2.txt
