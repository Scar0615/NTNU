p = [
    [1,0,0,0],
    [0,1,0,0],
    [0,0,1,0],
    [0,0,0,1]
]

l = [
    [1,0,0,0],
    [0,1,0,0],
    [0,0,1,0],
    [0,0,0,1]
]

u = [
    [0,0,-1,1],
    [1,1,-1,2],
    [-1,-1,2,0],
    [1,2,0,2],
]

n = 4

# 求出P矩陣
## 先數出每個row有多少非零元素
nonZeroCntList = []
for i in range(n):
    nonZeroCnt = 0
    for j in range(n):
        if u[i][j] != 0:
            nonZeroCnt += 1
    nonZeroCntList.append(nonZeroCnt)

for i in range(4):
    for j in range(i+1,4):
        if nonZeroCntList[j] >= nonZeroCntList[i]:
            nonZeroCntList[i],nonZeroCntList[j] = nonZeroCntList[j],nonZeroCntList[i]
            u[i],u[j] = u[j],u[i]
            p[i],p[j] = p[j],p[i]

# 求出LU矩陣
for i in range(n):
    ## 開始逐行消係數
    for j in range(i+1,n):
        multiple = u[j][i] / u[i][i]
        for k in range(i, n):
            u[j][k] -= u[i][k] * multiple
            l[j][k] += l[i][k] * multiple

# 輸出L跟U矩陣
print('P matrix:')
for i in p:
    print(i)
print('L matrix:')
for i in l:
    print(i)
print('U matrix:')
for i in u:
    print(i)