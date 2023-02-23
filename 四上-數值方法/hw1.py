import math

a, b, ot, border = 3, 4, 1/3, 10**(-6)
x = (a+b)/2
fx = x - math.pow(x, ot) - 2
# 終止條件: -10^-4 < f(x) < 10^-4
while fx < border*(-1) or fx > border:
    if fx > 0:
        b = x
        x = (a+b)/2
    elif fx < 0:
        a = x
        x = (a+b)/2
    fx = x - pow(x, ot) - 2
    print(round(x,4))