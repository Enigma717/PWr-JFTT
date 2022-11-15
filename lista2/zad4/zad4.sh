# !/bin/bash

lex -o zad4.c zad4.l
g++ zad4.c -o zad4
./zad4 < test.txt > results.txt
