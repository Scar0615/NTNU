x = [0.6, 0.7, 0.8, 0.9, 1]
y = [1.433329, 1.632316, 1.896481, 2.247908, 2.718282]
size = 5

# Newton's Divided Differences -> N.D.D.
def ndd(a, b, num):
    num -= 1
    mole = f[b][b+num] - f[a][a+num]
    deno = x[b+num] - x[a]
    return (mole / deno)

# Original
def each(i, value):
	pro = 1;
	for j in range(i):
		pro = pro * (value - x[j]);
	return pro

def p(value, n):
	sum = f[0][0]
	for i in range(1, n):
		sum = sum + (each(i, value) * f[0][i])
	return sum
        

# 建構二維陣列
# f = [
#     [0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0],
# ]
f = [
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0]
]

# 填入初始值
for i in range(size):
    f[i][i] = y[i]

for num in range(1, size):
    for i in range(0, size-num):
        f[i][i+num] = ndd(i, i+1, num)

for i in range(size):
    print(f[i])

# 算出解答
# print('P(0.82) =',p(0.82, size))
# print('P(0.98) =',p(0.98, size))