/*
    Autor:      Marek Traczyński (261748)
    Kurs:       Języki Formalne i Techniki Translacji
    Projekt:    Kompilator
    Opis pliku: Operacje arytmetyczne
*/



long long int COM_add(long long int x, long long int y)
{
    long long int result = x + y;
    std::cout << " (Result: " << result << ")";

    return result;
}

long long int COM_sub(long long int x, long long int y)
{
    long long int result = x - y;
    std::cout << " (Result: " << result << ")";

    return result;
}

long long int COM_mul(long long int x, long long int y)
{
    long long int result = x * y;
    std::cout << " (Result: " << result << ")";

    return result;
}

long long int COM_div(long long int x, long long int y)
{
    long long int result = x / y;
    std::cout << " (Result: " << result << ")";

    return result;
}

long long int COM_mod(long long int x, long long int y)
{
    long long int result = x % y;
    std::cout << " (Result: " << result << ")";

    return result;
}