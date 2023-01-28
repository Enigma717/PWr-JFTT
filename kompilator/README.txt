Projekt: Kompilator prostego języka imperatywnego
Autor: Marek Traczyński (261748)

----------------------------------
Użyte narzędzia:
    - g++ 12.2.0
    - bison (GNU Bison) 3.8.2
    - flex 2.6.4
    - GNU Make 4.4

----------------------------------
Kompilacja:
    - Do skompilowania całego projektu wystarczy użyć polecenia "make" z poziomu głównego folderu
    - Projekt był kompilowany i testowany pod standardem C++17 na systemie Arch Linux w wersji Linux 6.1.3-arch1-1
    - Wynikiem kompilacji za pomocą "make" powinien być plik wykonywalny "compiler"

----------------------------------
Uruchamianie kompilatora:
    - Kompilator uruchamiany jest poleceniem:
        $ ./compiler [input] [output]
    - Warunkiem koniecznym działania programu jest ścieżka pliku źródłowego "[input]"
    - Drugi argument "[output]", będący ścieżką pliku generowanego przez kompilator, jest opcjonalny
    - W przypadku braku argumentu "[output]" domyślnie generowany jest plik "./output.mr"

----------------------------------
Dołączone pliki:
    - asm/
        - asm.cpp, asm.h -> implementacja klasy pojedynczej instrukcji asemblera maszyny wirtualnej oraz funkcji generujących kod wynikowy
    - ast/
        - astree.cpp, astree.h -> implementacja klasy z drzewem składniowym AST oraz wszystkie funkcje generujące nowe poddrzewa
    - frontend/
        - lexer.l -> analizator leksykalny plików z kodem źródłowym kompilowanego języka imperatywnego
        - parser.ypp -> parser gramatyki ww. jezyka, w którym budujemy drzewo AST wywołując poszczególne funkcje w odpowiednich produkcjach
    - symtab/
        - symtab.cpp, symtab.h -> implementacja funkcji do działania na tablicy symboli generowanej przez lexer i parser
        - symtabnode.cpp, symtabnode.h -> implementacje klas składających się na węzły poszczególnych elementów w tablicy symboli
    - main.cpp -> główny plik kompilatora, z którego wywoływany jest parser (bison) oraz generowanie kodu asemblera
    - Makefile
    - README.txt
