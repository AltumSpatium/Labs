import numpy as np

n = 5
m = 3

a = np.array([[20, 30, 25]], dtype=float)
b = np.array([[10, 10, 10, 10, 10]], dtype=float)

c = np.array([[2, 8, -5, 7, 10],
			  [11, 5, 8, -8, -4],
			  [1, 3, 7, 4, 2]], dtype=float)

U = [(i, j) for i in range(m) for j in range(n)]

def check_balance():
	global n, m, a, b, c
	sum_a = sum(a[0])
	sum_b = sum(b[0])

	if sum_a == sum_b:
		return
	if sum_a > sum_b:
		n += 1
		b = np.append(b, [[sum_a - sum_b]], axis=1)
		c = np.append(c, [[0], [0], [0]], axis=1)
	else:
		m += 1
		a = np.append(a, [[sum_b - sum_a]], axis=1)
		c = np.append(c, [[0 for _ in range(n)]], axis=0)
