import sympy
import gauss

def main():
	A = sympy.Matrix([[3, 11], [22, 6]])
	b = sympy.Matrix([0, 0])
	print("Исходная матрица A:")
	sympy.pprint(A)
	x = gauss.gauss(A, b)
	if x:
		print("\nРезультат:", x)

if __name__ == "__main__":
	main()