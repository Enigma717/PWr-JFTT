# !/bin/bash

lex -o zad2.c zad2.l
gcc zad2.c -o zad2
./zad2 < test.py > lexed.py
