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
	global U_n
	U_n = [cell for cell in U if not cell in U_b]


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


def calc_potentials():
	u = [None for _ in range(m)]
	v = [None for _ in range(n)]
	u[0] = 0
	while True:
		for cell in U_b:
			i, j = cell
			if u[i] is not None:
				if v[j] is None:
					v[j] = c[cell] - u[i]
			elif v[j] is not None:
				if u[i] is None:
					u[i] = c[cell] - v[j]
		if (not None in u) and (not None in v):
			break
	return (u, v)


def calc_estimates(u, v):
	delta = np.ndarray((m, n))
	delta.fill(float('inf'))
	for cell in U_n:
		delta[cell] = c[cell] - u[cell[0]] - v[cell[1]]
	return delta


def append_cell(U_b, cell):
	U_b_copy = U_b[:]
	U_b_copy.append(cell)
	return U_b_copy


def create_cycle(new_cell):
	def _find_cycle(prev_cell, cycle, U_b_ex, vertical):
		cycle.append(prev_cell)
		direction = 1 if vertical else 0
		result = None

		cells_to_visit = [cell for cell in U_b_ex if cell not in cycle \
			and cell[direction] == prev_cell[direction]]
		for cell in cells_to_visit:
			result = _find_cycle(cell, cycle[:], U_b_ex, not vertical)
			if result:
				return result

		if (prev_cell[0] == new_cell[0] or prev_cell[1] == new_cell[1]) \
			and len(cycle) >= 4 and len(cycle) % 2 == 0:
			return cycle

	return _find_cycle(new_cell, [], append_cell(U_b, new_cell), True)


def get_step(cycle):
	global x
	U_b_minus = cycle[1::2]
	theta = min([x[cell] for cell in U_b_minus])
	for cell in U_b_minus:
		if x[cell] == theta:
			return (theta, cell)


def new_transportation_plan(theta, cycle):
	for i in range(len(cycle)):
		if i % 2 == 0:
			x[cycle[i]] += theta
		else:
			x[cycle[i]] -= theta


def new_basis_plan(ij_0, ij_s):
	U_b.remove(ij_s)
	U_b.append(ij_0)
	U_b.sort()


def potentials_method():
	while True:
		u, v = calc_potentials()
		delta = calc_estimates(u, v)
		i_0 = delta.argmin() // n
		ij_0 = (i_0, delta.argmin() - n * i_0)
		if delta[ij_0] >= 0:
			print('Оптимальный план:')
			print(x)
			break
		else:
			cycle = create_cycle(ij_0)
			theta, ij_s = get_step(cycle)
			new_transportation_plan(theta, cycle)
			new_basis_plan(ij_0, ij_s)
			calc_nonbasis_cells()

		
def main():
	global U
	check_balance()
	U = [(i, j) for i in range(m) for j in range(n)]
	basis_transportation_plan()
	calc_nonbasis_cells()
	correct_basis_plan()
	potentials_method()


if __name__ == '__main__':
	main()
