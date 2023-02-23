# 要求的是圓心(h,k), 半徑r
def gen_f(x, y, ans):
    return ((x ** 2 - 2 * ans[0] * x) + (y ** 2 - 2 * y * ans[1]) + (ans[0] ** 2 + ans[1] ** 2 - ans[2] ** 2))

# 算出D_F矩陣
def gen_d(x, y, ans):
    row = []
    row.append(x * (-2) + 2 * ans[0])
    row.append(y * (-2) + 2 * ans[1])
    row.append(ans[2] * (-2))
    return row

# 丟入一個空的list(matrix)
def matrix_generate(size, matrix):
    for i in range(size):
        newlist = []
        for j in range(size):
            newlist.append(0)
        matrix.append(newlist)
    return matrix

# 檢查成長值是否穩定
def check(s):
    flag = 0
    for i in range(amount):
        if abs(s[i]) <= 10**(-6):
            flag += 1
    return flag

# 一開始對h,k,r進行假設
# ans = [h, k, r]

# 第一題的假設
ans = [0, 0, 10]

# 第二題的假設
# ans = [-1, 0, 6]
amount = 3      # 需要求的變數有h,k,r，共3個。

while 1:
    d = []
    f = []

    # 第一題的x,y
    x = [-8, 6, 4]
    y = [-4, 9, -9]

    # 第二題的x,y
    # x = [-1, -2, 5]
    # y = [6, -6, 0]
    for i in range(3):
        d.append(gen_d(x[i], y[i], ans))
        f.append(gen_f(x[i], y[i], ans))

    # 為了要算出迭代的h,k,r，需要對D做高斯消去。
    ## 開始逐行消係數
    for i in range(amount):
        for j in range(i+1, amount):
            multiple = d[j][i] / d[i][i]
            f[j] -= f[i] * multiple
            for k in range(i, amount):
                d[j][k] -= d[i][k] * multiple

    for i in range(amount):
        f[i] *= -1

    # 解出變數
    s = [0, 0, 0]
    for i in range(amount-1, -1, -1):
        s[i] = f[i] / d[i][i]   # 算出變數的解
        for j in range(i-1, -1, -1):
            multiple = d[j][i] / d[i][i]
            d[j][i] -= d[i][i] * multiple
            f[j] -= s[i] * multiple

    if check(s) == amount:
        break
    for i in range(amount):
        ans[i] += s[i]

print(ans)