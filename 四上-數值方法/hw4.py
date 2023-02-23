# 丟入一個空的list(matrix)
def matrix_generate(size, matrix):
    for i in range(size):
        newlist = []
        for j in range(size+1):
            newlist.append(0)
        matrix.append(newlist)
    return matrix
        

amount = int(input('請輸入共有多少個變數: '))
u = []
# 輸入矩陣元素
for i in range(amount):
    newlist = list(map(int,input('請輸入第%d列的係數以及b: '%(i+1)).split()))
    u.append(newlist)

# 初始化L矩陣
l = matrix_generate(amount, [])
for i in range(amount):
    for j in range(amount):
        if i == j:
            l[i][j] = 1
        else:
            l[i][j] = 0

# 開始逐行消係數
for i in range(amount):
    for j in range(i+1, amount):
        multiple = u[j][i] / u[i][i]
        for k in range(i, amount+1):
            u[j][k] -= u[i][k] * multiple
            l[j][k] += l[i][k] * multiple

# 輸出L跟U矩陣
print('L matrix:')
for i in range(amount):
    print(l[i])

print('U matrix:')
for i in range(amount):
    print(u[i])

# 解出變數
ans = []
for i in range(amount-1, -1, -1):
    ans.append(u[i][amount]/u[i][i])
    for j in range(i-1, -1, -1):
        multiple = u[j][i] / u[i][i]
        u[j][i] -= u[i][i] * multiple
        u[j][amount] -= u[i][amount] * multiple

# 輸出結果
ansl = ['x','y','z','w']
for i in range(amount):
    print(ansl[i],'=',ans[i])