# x^4+3(x-2)^3-15x^2+1 -> x^4+3x^3-33x^2+36x-23 -> 4x^3+9x^2-66x+36

x = -5.5
lr = 0.01  #learning rate
while 1:
    change = lr*(4*(x**3) - 9*(x**2) - 66*x + 36)
    x_new = x - change
    if change <= 10**(-6):
        print(x_new)
        break
    x = x_new