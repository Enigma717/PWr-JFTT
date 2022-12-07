ORDER = 1234577

def f_add(x, y, order):
    return (x + y) % order

def f_sub(x, y, order):
    return (order + (x - y) % order) % order

def f_mul(x, y, order):
    return (x * y) % order

def f_div(x, y, order):
    try:
        return (x * pow(y, -1, order)) % order
    except ValueError as ve:
        print("ERROR: %d nie jest odwracalne modulo %d" % (y, order))

def f_pow(base, exponent, order):
    x = base
    y = 1
    n = exponent

    while n > 0:
        if n % 2 == 1:
            n = n - 1
            y = (y * x) % order
            
        n = n // 2
        x = (x * x) % order

    return y