import sympy
import gauss

def main():
	A = sympy.Matrix([[1, -3, -1], [2, -2, 1], [3, -1, 2]])
	b = sympy.Matrix([-11, -7, -4])
	print("Исходная матрица A:")
	sympy.pprint(A)
	print("\nВектор b:")
	sympy.pprint(b)
	x = gauss.gauss(A, b)
	if x:
		print("\nРезультат:", x)

if __name__ == "__main__":
	main()