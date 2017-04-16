import numpy as np
import numpy.linalg as la

INF = float('inf')

# Task 1
n = 8
m = 3

A = np.array([[11, 0, 0, 1, 0, -4, -1, 1],
			  [1, 1, 0, 0, 1, -1, -1, 1],
			  [1, 1, 1, 0, 1, 2, -2, 1]], dtype=float)
b = np.array([8, 2, 5], dtype=float)
B = np.array([[1, -1, 0, 3, -1, 5, -2, 1],
			  [2, 5, 0, 0, -1, 4, 0, 0],
			  [-1, 3, 0, 5, 4, -1, -2, 1]], dtype=float)
d = np.array([6, 10, 9], dtype=float).transpose()
D = B.transpose().dot(B)
c = -d.dot(B)

x = np.array([0.7273, 1.2727, 3, 0, 0, 0, 0, 0], dtype=float)
J = [i for i in range(n)]
J_b = [0, 1, 2]
J_s = [0, 1, 2]


def calc_cost_vector(D, x, c):
	return c + D.dot(x)


def get_basic_matrix(A, J_b):
	return A.take(J_b, axis=1)


def get_basic_cost(c_, J_b):
	return c_.take(J_b, axis=0)


def calc_potentials_vector(c_b_, A_b):
	return -c_b_.dot(la.inv(A_b))


def calc_estimates(u, A, c_):
	return u.dot(A) + c_
	

def check_estimates(delta, J, J_s):
	J_ns = np.delete(np.arange(n), J_s)
	if (delta[J_ns] >= 0).all():
		return (-1, True)
	return (J_ns[delta[J_ns].argmin()], False)


def calc_directions(J_s, j_0, D, A):
	l = np.zeros(n)
	l[j_0] = 1

	A_s = A.take(J_s, axis=1)
	D_s = np.take(np.take(D, J_s, axis=0), J_s, axis=1)
	O = np.zeros((m, m))
	H = np.vstack((np.hstack((D_s, A_s.transpose())), np.hstack((A_s, O))))
	bb = np.hstack((D.take(J_s, axis=0).take(j_0, axis=1), A.take(j_0, axis=1)))

	ldy = -la.inv(H).dot(bb)
	l[J_s] = ldy[:len(J_s)]
	return l


def calc_steps(l, x, J_s, j_0, delta, D):
	theta = [INF if l[j] >= 0 else -x[j]/l[j] for j in J_s]
	dlt = l.transpose().dot(D).dot(l)
	theta_j_0 = INF if 0 < abs(dlt) <= 1.0e-10 else abs(delta[j_0]) / dlt
	theta = np.array(theta, dtype=float)
	return (dlt, theta, theta_j_0)


def check_case_c(e, A, A_b, J_s, J_b):
	J_ex = [j for j in J_s if j not in J_b]
	for j in J_ex:
		if np.dot(np.dot(e, la.inv(A_b)), np.take(A, j, axis=1)) != 0:
			return (j, True)
	return (-1, False)

while True:
	c_ = calc_cost_vector(D, x, c)
	A_b = get_basic_matrix(A, J_b)
	c_b_ = get_basic_cost(c_, J_b)
	u = calc_potentials_vector(c_b_, A_b)
	delta = calc_estimates(u, A, c_)
	j_0, is_stop = check_estimates(delta, J, J_s)
	if is_stop:
		print('Оптимальный план:')
		print(list(map(lambda x: round(x, 4), x)))
		print('Целевая функция:')
		print(c.dot(x) + 0.5 * x.dot(D).dot(x))
		break
	else:
		while True:
			l = calc_directions(J_s, j_0, D, A) # step 3
			dlt, theta, theta_j_0 = calc_steps(l, x, J_s, j_0, delta, D)
			j_s = theta.argmin()
			theta_0, j_s = (theta_j_0, j_0) if theta[j_s] >= theta_j_0 else (theta[j_s], J_s[j_s])
			if theta_0 == INF:
				raise ValueError('Нет решения')
			x = x + theta_0 * l

			if j_s == j_0: # a
				J_s.append(j_0)
				break
			elif j_s in set(J_s) - set(J_b): # b
				J_s.remove(j_s)
				delta[j_0] = delta[j_0] + theta_0 * dlt
			else:
				s = J_b.index(j_s)
				e_s = np.eye(m)[s]
				j_p, case_c = check_case_c(e_s, A, A_b, J_s, J_b)
				if case_c: # c
					J_b.remove(j_s)
					J_b.append(j_p)
					J_s.remove(j_s)
					delta[j_0] = delta[j_0] + theta_0 * dlt
				else: # d
					J_b.remove(j_s)
					J_b.append(j_0)
					J_s.remove(j_s)
					J_s.append(j_0)
				J_b.sort()
				J_s.sort()
