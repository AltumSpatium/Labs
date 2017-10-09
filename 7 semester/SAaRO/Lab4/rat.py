import numpy as np


def rat(task):
    f = task[0]
    B = np.zeros(f.shape)
    Z = np.zeros((f.shape[0] - 1, f.shape[1]))

    n = f.shape[0]
    c = f.shape[1]

    B[0] = f[0]

    for i in range(1, n):
        for j in range(1, c):
            b = [B[i-1, j - z] + f[i, z] for z in range(j+1)]
            Z[i - 1][j] = b.index(max(b))
            B[i, j] = max(b)

    max_gain = B[n-1, c-1]
    res_left = c - 1
    result = []

    for i in range(1, n):
        z = int(Z[n-i-1, res_left])
        result.insert(0, z)
        res_left -= z

    result.insert(0, res_left)
    return (result, B, Z)
