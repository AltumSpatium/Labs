import sympy
import gauss

def main():
	A = sympy.Matrix([[3, 4, 5], [1, -1, -2], [2, 1, 2], [3, -3, 1], [1, 2, 2]])
	b = sympy.Matrix([7, 0, 3, 0, 5])
	print("Исходная матрица A:")
	sympy.pprint(A)
	print("\nВектор b:")
	sympy.pprint(b)
	x = gauss.gauss(A, b)
	if x:
		print("\nРезультат:", x)

if __name__ == "__main__":
	main()