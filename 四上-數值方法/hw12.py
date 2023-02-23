import math
import numpy as np

# 算出Dr(x)
def gen_dr(ans):
    # Si = math.sqrt((x-xi)**2 + (y-yi)**2)
    S1 = math.sqrt((ans[0]+1)**2 + ans[1]**2)
    S2 = math.sqrt((ans[0]-1)**2 + (ans[1]-0.5)**2)
    S3 = math.sqrt((ans[0]-1)**2 + (ans[1]+0.5)**2)
    S4 = math.sqrt(ans[0]**2 + (ans[1]-1)**2)
    # row of dr: [(x-xi)/Si, (y-yi)/Si, -1]
    dr = [
        [(ans[0]+1)/S1, ans[1]/S1, -1],
        [(ans[0]-1)/S2, (ans[1]-0.5)/S2, -1],
        [(ans[0]-1)/S3, (ans[1]+0.5)/S3, -1],
        [ans[0]/S4, (ans[1]-1)/S4, -1]
    ]
    return dr

# 算出r(x)
def gen_r(ans):
    # ri(x,y) = math.sqrt((x-xi)**2 + (y-yi)**2) - (ri + k)
    r = [
        math.sqrt((ans[0]+1)**2+ans[1]**2)-(1+ans[2]),
        math.sqrt((ans[0]-1)**2+(ans[1]-0.5)**2)-(0.5+ans[2]),
        math.sqrt((ans[0]-1)**2+(ans[1]+0.5)**2)-(0.5+ans[2]),
        math.sqrt(ans[0]**2+(ans[1]-1)**2)-(0.5+ans[2])
    ]
    return r

# 轉置矩陣
def t(A, m, n):
    AT = []
    for i in range(n):
        tmp = []
        for j in range(m):
            tmp.append(A[j][i])
        AT.append(tmp)
    return AT

# 初始答案(x,y,k)
row = 4
col = 3
ans = [0.5, 0.5, 0.5]
while 1:
    dr = gen_dr(ans)
    r = gen_r(ans)
    drt = t(dr, row, col)
    drtdr = np.matmul(drt, dr)
    drtr = np.matmul(drt, r)
    idrtdr = np.linalg.inv(drtdr)
    v = np.matmul(idrtdr, drtr) * (-1)
    ans = ans + v
    if abs(max(v)) < 10**(-6):
        print(ans)
        break