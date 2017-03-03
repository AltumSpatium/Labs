import numpy as np
import numpy.linalg as la

INF = float('inf')

def inverse_basis_matrix(A_b):
    return la.inv(A_b)

def calc_new_basis_indexes(J_b, k, j_s):
    J_b[k] = j_s
    return J_b

def nonbasis_indexes(J, J_b):
    return [j for j in J if j not in J_b]

def basis_matrix(A, J_b):
    return np.take(A, J_b, axis=1)

def calc_coplan(y, A, c):
    return np.dot(y, A) - c

def calc_basis_pseudoplan(B, b):
    return np.dot(B, b)

def check_pseudoplan(kappa):
    for elem in kappa:
        if elem < 0:
            return False
    return True

def calc_mu(delta_y, A_j):
    return np.dot(delta_y, A_j)

def calc_sigma(mu, delta, J_n):
    sigma = []
    for j in J_n:
        sigma_j = INF if mu[j] >= 0 else -delta[j]/mu[j]
        sigma.append(sigma_j)
    return sigma

def calc_new_dual_plan(y, sigma_0, delta_y):
    return y + np.dot(sigma_0, delta_y)

def get_optimal_plan(x, kappa, J_b):
    for i in range(m):
        x[J_b[i]] = kappa[i]
    return x

# Task 1
n = 8
m = 3
J = [i for i in range(n)]
c = np.array([5, 2, 3, -16, 1, 3, -3, -12], dtype=float)
b = np.array([-2, -4, -2], dtype = float)
A = np.array([[-2, -1, 1, -7, 0, 0, 0, 2], 
              [4, 2, 1, 0, 1, 5, -1, -5],
              [1, 1, 0, -1, 0, 3, -1, 1]], dtype=float)
x = np.zeros(n, dtype=float)

J_b = [0, 1, 2]
J_n = nonbasis_indexes(J, J_b)

y = np.array([1, 2, -1], dtype=float)

while True:
    delta = calc_coplan(y, A, c)
    A_b = basis_matrix(A, J_b)
    B = inverse_basis_matrix(A_b)

    kappa = calc_basis_pseudoplan(B, b)
    if check_pseudoplan(kappa):
        x = get_optimal_plan(x, kappa, J_b)
        print('Оптимальный план:')
        print(x)
        print('Прибыль: ', sum(map(lambda a, b: a*b, c, x)))
        break
    else:
        k = kappa.argmin()
        delta_y = B.take(k, axis=0)
        mu = calc_mu(delta_y, A)

        sigma = calc_sigma(mu, delta, J_n)
        sigma_0 = min(sigma)
        if sigma_0 == INF:
            print("Задача не имеет решения")
            break
        else:
            j_s = J_n[sigma.index(sigma_0)]
            y = calc_new_dual_plan(y, sigma_0, delta_y)
            J_b = calc_new_basis_indexes(J_b, k, j_s)
            J_n = nonbasis_indexes(J, J_b)
