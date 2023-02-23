import numpy as np

# Jacobi Method
# x=D^-1(b-(L+U)x)
size = 10
empty_1 = [[0]*size]*size
empty = np.array(empty_1)
D = empty.copy()
L = empty.copy()
U = empty.copy()
D_inv = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
]
b = np.array([[2], [3], [4], [5], [6], [7], [8], [9], [10], [11]])
x = np.array([[1], [1], [1], [1], [1], [1], [1], [1], [1], [1]])

def g(x, D, L, U, b):
    for i in range(10):
        D_inv[i][i] = 1/D[i][i]
        LU = L+U
    return np.matmul(D_inv,(b-np.matmul(LU,x)))

def check(new, prev):
    flag = 0
    for i in range(size):
        if abs(new[i]-prev[i]) <= 10**(-6):
            flag += 1
    return flag

# 先把D,L,U矩陣算出來
for i in range(10):
    for j in range(10):
        if i == j:
            D[i][j] = -4
        elif j-i == 1:
            U[i][j] = 2
        elif i-j == 1:
            L[i][j] = 2

npd = np.array(D)
npu = np.array(U)
npl = np.array(L)

while 1:
    new_x = g(x, npd, npl, npu, b)
    if check(new_x, x) == size:
        break
    x = new_x

print(new_x)

# Gauss-Seidel Method
# x_k+1=D^-1(b-Ux_k-Lx_k+1)