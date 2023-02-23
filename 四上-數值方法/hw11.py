import math

v = [
    [4,2,3,0],
    [-2,3,-1,1],
    [1,3,-4,2],
    [1,0,1,-1],
    [3,1,3,-2]
]

u = [
    [0,0,0,0],
    [0,0,0,0],
    [0,0,0,0],
    [0,0,0,0],
    [0,0,0,0]
]

r = [
    [0,0,0,0],
    [0,0,0,0],
    [0,0,0,0],
    [0,0,0,0]
]

col = 5
row = 4

def mul(v, u, nv, nu):
    val = 0
    for i in range(col):
        val += (v[i][nv]*u[i][nu])
    return val

def dist(u):
    dist_u = []
    for i in range(row):
        tmp = 0
        for j in range(col):
            tmp += u[j][i]**2
        dist_u.append(math.sqrt(tmp))
    return dist_u

for i in range(row):
    # u1 = v1
    if i == 0:
        for j in range(col):
            u[j][i] = v[j][i]
    else:
        # 先令un = vn
        for j in range(col):
            u[j][i] = v[j][i]
        # 減去(<vm,un>/<un,un>)un
        for j in range(i):
            for k in range(col):
                u[k][i] -= mul(v, u, i, j)/mul(u, u, j, j)*u[k][j]

# 產生R矩陣
for i in range(row):
    for j in range(row):
        if i == j:
            r[i][j] = 1
        elif i < j:
            r[i][j] = mul(v, u, j, i)/mul(u, u, i, i)

# 將U矩陣和R矩陣調整成QR
du = dist(u)
for i in range(row):
    for j in range(col):
        u[j][i] /= du[i]
    for j in range(row):
        r[i][j] *= du[i]

print('Matrix Q:')
for i in range(col):
    print(u[i])

print('Matrix R:')
for i in range(row):
    print(r[i])