point = [
    [1.0, 0.1924],
    [1.05, 0.2414],
    [1.1, 0.2933],
    [1.15, 0.3492],
]

n = 4

def l(order, x):
    mole = 1 # 分子(molecular)
    deno = 1 # 分母(denominator)
    for i in range(n):
        if i == order:
            continue
        mole *= (x - point[i][0])
        deno *= (point[order][0] - point[i][0])
    return (mole/deno)

px = 0
for i in range(n):
    px += point[i][1] * l(i, 1.09)

print(px)