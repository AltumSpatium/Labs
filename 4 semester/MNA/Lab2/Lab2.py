import sympy
from sympy import pprint

def main():
	# Variables initialization:

	A = sympy.Matrix([[25.43, 2.42, 4.85],
					  [2.31, 29.12, 3.52],
					  [2.12, 4.85, 28.92]])

	b = sympy.Matrix([[10.41],
					  [20.43],
					  [12.34]])

	c = sympy.Matrix([[0], [0], [0]])
	B = sympy.Matrix([[0, 0, 0], [0, 0, 0], [0, 0, 0]])
	x = sympy.zeros(3, 1)
	y = sympy.zeros(3, 1)
	e = 0.00001
	n = A.rows

	# Computing matrices B and c:

	for i in range(n):
		c[i] = b[i] / A[i, i]
		for j in range(n):
			B[i, j] = -A[i, j] / A[i, i]
		B[i, i] = 0

	print("\nMatrix B:\n")
	pprint(B)

	print("\nMatrix c:\n")
	pprint(c)

	#Computing matrix norm:

	norm = [0, 0, 0]
	for i in range(n):
		for j in range(n):
			norm[i] = norm[i] + abs(B[i, j])

	print("\nNorm:", max(norm))
	print()
	if (max(norm) > 1):
		print("The iterative sequence diverges!")
		return

	#Using MSI:

	print("MSI:\n")

	xk = sympy.zeros(3, 1)
	xk = c
	x = B * xk + c
	iter_count_msi = 0
	while abs(max(x - xk)) > e:
		xk = x
		x = B * xk + c
		iter_count_msi += 1

	pprint(x)
	print("\nIteration count:", iter_count_msi)

	#Using Seidel method:

	print("\nSeidel:\n")

	yk = sympy.zeros(3, 1)
	yk = c
	y = B * yk + c
	iter_count_seid = 1
	while abs(max(y - yk)) > e:
		yk = y
		for i in range(n):
			y[i] = 0
			for j in range(i):
				y[i] = y[i] + B[i, j] * y[j]
			for j in range(i, n):
				y[i] = y[i] + B[i, j] * yk[j]
			y[i] = y[i] + c[i]
		iter_count_seid += 1

	pprint(y)
	print("\nIteration count:", iter_count_seid)

if __name__ == "__main__":
	main()