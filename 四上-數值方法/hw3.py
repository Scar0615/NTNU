# 我是用Newton
import math

def e(x,gx):
    error = x - gx
    if error < 0:
        error *= -1
    return error

x = 0
# Q1: f'(x) = 5x^4+1
f1x = x - (x**5 + x - 1)/(5 * x**4 + 1)
while e(x,f1x) >= 10**(-6):
    x = f1x
    f1x = x - (x**5 + x - 1)/(5 * x**4 + 1)
print('Root of Q1 is',x)

# Q2: f'(x) = 1/x+2x
f2x = x - (math.log(x) + x**2 - 3)/(x**(-1) + 2*x)
while e(x,f2x) >= 10**(-6):
    x = f2x
    f2x = x - (math.log(x) + x**2 - 3)/(x**(-1) + 2*x)
print('Root of Q2 is',x)

# Q3: f'(x) = cosx-6
f3x = x - (math.sin(x) - 6*x - 5)/(math.cos(x) - 6)
while e(x,f3x) >= 10**(-6):
    x = f3x
    f3x = x - (math.sin(x) - 6*x - 5)/(math.cos(x) - 6)
print('Root of Q3 is',x)