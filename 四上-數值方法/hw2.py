import math

def e(x,gx):
    error = x - gx
    if error < 0:
        error *= -1
    return error

x = 1.5
gx = math.pow(2*x + 2,1/3)
while e(x,gx) >= 10**(-6):
    x = gx
    gx = math.pow(2*x + 2,1/3)
    print(x)
print('Root is %f'%x)