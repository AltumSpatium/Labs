import sympy

def gauss(A, b):
	try:
		n = A.rows
		m = A.cols
		for j in range(n - 1):
			for i in range(j + 1, n):
				curr = A[i,j]
				b[i] = b[i] - (curr * b[j]) / A[j,j]
				for k in range(j, m):
					A[i,k] -= (curr * A[j,k]) / A[j,j]

		x = [sympy.Symbol("x" + str(i)) for i in range(1, m + 1)]

		for i in range(A.rank() - 1, -1, -1):
			x[i] = b[i]
			for j in range(m - 1, i, -1):
				x[i] -= A[i,j] * x[j]
			x[i] /= A[i,i]

		return x
	except Exception:
		print("\nСистема несовместна!")