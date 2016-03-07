import sympy
import gauss

def main():
	a = sympy.Symbol('a')
	A = sympy.Matrix([[3, a, -3], [a, -3, 1], [5, -1, -2]])
	print("Исходная матрица A:")
	sympy.pprint(A)
	aVal = sympy.solve(sympy.Eq(A.det(), 0), a)
	print("\nЗначение а:", aVal)
	for i in aVal:
		print("\nПри а =", i)
		Asubs = A.subs(a, i)
		b = sympy.zeros(1, A.rows)
		x = gauss.gauss(Asubs, b)
		if x:
			print("\nРезультат:", x)

if __name__ == "__main__":
	main()