import sympy
import gauss

def main():
	A = sympy.Matrix([[7, 1, 7], [-1, 7, 1], [7, -1, 7]])
	b = sympy.Matrix([15, 5, 13])
	print("Исходная матрица A:")
	sympy.pprint(A)
	print("\nВектор b:")
	sympy.pprint(b)
	x = gauss.gauss(A, b)
	if x:
		print("\nРезультат:", x)

if __name__ == "__main__":
	main()