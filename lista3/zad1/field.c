#include <stdio.h>
#include "field.h"


const long long int ORDER = 1234577;


long long int f_add(long long int x, long long int y, long long int order)
{
    long long int result = (x + y) % order;

    return result;
}

long long int f_sub(long long int x, long long int y, long long int order)
{
    long long int result = (order + (x - y) % order) % order;

    return result;
}

long long int f_mul(long long int x, long long int y, long long int order)
{
    long long int result = (x * y) % order;

    return result;
}

long long int f_div(long long int x, long long int y, long long int order)
{
    long long int result = (x * f_pow(y, order - 2, order)) % order;

    return result;
}

long long int f_pow(long long int base, long long int exponent, long long int order)
{
    long long int x = base;
    long long int y = 1;
    long long int n = exponent;

    while (n > 0)
    {
        if (n % 2 == 1)
        {
            y = (y * x) % order;
        }
        n /= 2;
        x = (x * x) % order;
    }

    return y;
}
