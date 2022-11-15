# !/bin/bash

lex -o zad3.c zad3.l
gcc zad3.c -o zad3
./zad3 < test.cpp > lexed.cpp
./zad3 dox < test.cpp > lexeddox.cpp
