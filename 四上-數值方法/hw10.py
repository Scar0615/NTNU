import numpy as np

def t(A, m, n):
    AT = []
    for i in range(n):
        tmp = []
        for j in range(m):
            tmp.append(A[j][i])
        AT.append(tmp)
    return AT

# xm: 矩陣x的row個數, xn: 矩陣x的column個數(ym=xn所以不用代入), yn: 矩陣y的column個數
def mat_mul(x, y, xm, xn, yn):
    # an和bm如果不相同，根本算不出來，所以只需代入am,bn。
    ans = []
    for i in range(xm):
        tmp = []
        for j in range(yn):
            temp = 0
            for k in range(xn):
                temp += x[i][k]*y[k][j]
            tmp.append(temp)
        ans.append(tmp)
    return ans

# 計算A矩陣
# A = []
# for i in range(13):
#     i /= 4
#     A.append([1, i, i**2, i**3])

# b = [[6.3806], [7.1338], [9.1662], [11.5545], [15.6414], [22.7371], [32.0696], [47.0756], [73.1596], [111.4684], [175.9895], [278.555], [446.4441]]

A = [[4],[7],[11]]
b = [[3],[5],[8]]
row = 3
col = 1
at = t(A, row, col)
ata = mat_mul(at, A, col, row, col)
atb = mat_mul(at, b, col, row, 1)

# num_ata = np.array([ata[0], ata[1], ata[2], ata[3]])
ata_inv = np.linalg.inv(ata)
num_atb = np.array(atb)
ans = ata_inv.dot(num_atb)
print(ans)
# print('f(x) = %fx^3 - %fx^2 + %fx - %f'%(ans[3], ans[2]*(-1), ans[1], ans[0]*(-1)))