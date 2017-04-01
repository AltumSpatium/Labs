import numpy as np


n = 5
m = 3

a = np.array([[20, 30, 25]], dtype=float)
b = np.array([[10, 10, 10, 10, 10]], dtype=float)

c = np.array([[2, 8, -5, 7, 10],
			  [11, 5, 8, -8, -4],
			  [1, 3, 7, 4, 2]], dtype=float)
x = None

U = []
U_b = []
U_n = []


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


def basis_transportation_plan():
	global x
	x = np.zeros((m, n))
	a_help = a.tolist()[0]
	b_help = b.tolist()[0]
	i = 0
	j = 0
	while i < m and j < n:
		a_i = a_help[i]
		b_j = b_help[j]
		x_ij = min(a_i, b_j)

		a_help[i] -= x_ij
		b_help[j] -= x_ij

		x[(i, j)] = x_ij
		U_b.append((i, j))
		if a_i == b_j:
			i += 1
			j += 1
		elif x_ij == a_i:
			i += 1
		else:
			j += 1


def calc_nonbasis_cells():
	for cell in U:
		if cell not in U_b:
			U_n.append(cell)


def correct_basis_plan():
	N = n + m - 1
	if len(U_b) == N:
		return
	elif (len(U_b) > N):
		raise ValueError()
	
	count = 0
	U_b_ = U_b[:]
	for _ in range(N - len(U_b)):
		U_n_ = U_n[count:]
		for cell in U_n_:
			count += 1
			if (cell[0] + 1, cell[1]) in U_b_ and (cell[0], cell[1] - 1) in U_b_:
				U_b.append(cell)
				U_n.remove(cell)
				break
	U_b.sort()


def test():
	global U_b
	del U_b[-2]
	U_b.append((0, 5))
	U_b.sort()
	U_n.append((1, 5))
	U_n.remove((0, 5))
	U_n.sort()


def calc_potentials():
	u = [None for _ in range(m)]
	v = [None for _ in range(n)]
	u[0] = 0
	while None in u and None in v:
		for cell in U_b:
			i, j = cell
			if u[i] is not None:
				if v[j] is None:
					v[j] = c[cell] - u[i]
			elif v[j] is not None:
				if u[i] is None:
					u[i] = c[cell] - v[j]
			
	return (u, v)


def calc_estimates(u, v):
	delta = np.ndarray((m, n))
	delta.fill(float('inf'))
	for cell in U_n:
		delta[cell] = c[cell] - u[cell[0]] - v[cell[1]]
	return delta


def append_cell(U_b, cell):
	U_b.append(cell)
	return U_b


def find_cycle(new_cell):
	def _find_cycle(prev_cell, cycle, U_b_ex, vertical):
		cycle.append(prev_cell)
		direction = 1 if vertical else 0
		result = None

		cells_to_visit = [cell for cell in U_b_ex if cell not in cycle
							and cell[direction] == prev_cell[direction]]
		for cell in cells_to_visit:
			result = _find_cycle(cell, cycle[:], U_b_ex, not vertical)
			if result:
				return result

		if (prev_cell[0] == new_cell[0] or prev_cell[1] == new_cell[1]) \
			and len(cycle) >= 4 and len(cycle) % 2 == 0:
			return cycle

	return _find_cycle(new_cell, [], append_cell(U_b, new_cell), True)


def potentials_method():
	u, v = calc_potentials()
	delta = calc_estimates(u, v)
	ij_0 = (delta.argmin() // n, delta.argmin() - n)
	if delta[ij_0] >= 0:
		print('Оптимальный план:')
		print(x)
		print('Расходы: ', sum(map(lambda a, b: a*b, c, x)))
	else:
		cycle = find_cycle(ij_0)
		print(cycle)

		
def main():
	global U
	check_balance()
	U = [(i, j) for i in range(m) for j in range(n)]
	basis_transportation_plan()
	calc_nonbasis_cells()
	correct_basis_plan()
	test()
	potentials_method()


if __name__ == '__main__':
	main()
