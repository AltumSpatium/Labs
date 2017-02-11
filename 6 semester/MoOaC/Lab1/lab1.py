import numpy as np
import numpy.linalg as la

INF = float('inf')

def basis_cost(c, J_b):
    return np.take(c, J_b)

def basis_plan(x, J_b):
    x_ = [x[j] for j in J_b]
    return np.array(x_, dtype=np.float)

def vector_of_potentials(c_b, B):
    return np.dot(c_b, B)

def estimates(U, A, c, J_n):
    delta = []
    for j in J:
        if j in J_n:
            delta.append(np.dot(U, A[:, j]) - c[j])
        else:
            delta.append(0)
    return delta

def inverse_basis_matrix(A_b):
    return la.inv(A_b)

def calc_steps(Z, x_b, m):
    theta = []
    for i in range(m):
        if Z[i] <= 0:
            theta.append(INF)
        else:
            theta.append(x_b[i] / Z[i])
    return theta

def calc_new_basis_indexes(J_b, j_0, s):
    J_b[s] = j_0
    return J_b

def calc_new_plan(x, J_b, J_n, j_0, Z, theta_0, m):
    x[J_n] = 0
    for i in range(m):
        x[J_b[i]] -= Z[i] * theta_0
    x[j_0] = theta_0
    return x

def nonbasis_indexes(J, J_b):
    return [j for j in J if j not in J_b]

def basis_matrix(A, J_b):
    return np.take(A, J_b, axis=1)

# Task 1 +
n = 8
m = 4
J = [i for i in range(n)]
c = np.array([-5, 2, 3, -4, -6, 0, 1, -5], dtype = float)
b = np.array([36, -11, 10, 20], dtype = float)
A = np.array([[0, 1, 4, 1, 0, -8, 1, 5], 
              [0, -1, 0, -1, 0, 0, 0, 0],
              [0, 2, -1, 0, -1, 3, -1, 0],
              [1, 1, 1, 1, 0, 3, 1, 1]], dtype = float)
x = np.array([4, 5, 0, 6, 0, 0, 0, 5], dtype = float)
J_b = [0, 1, 3, 7]
J_n = nonbasis_indexes(J, J_b)

while True:
    A_b = basis_matrix(A, J_b)
    x_b = basis_plan(x, J_b)
    c_b = basis_cost(c, J_b)
    B = inverse_basis_matrix(A_b)
    U = vector_of_potentials(c_b, B)
    delta = estimates(U, A, c, J_n)
    mindelta = min(delta)

    if (mindelta >= 0):
        print('Оптимальный план:')
        print(x)
        print('Прибыль: ', sum(map(lambda a, b: a*b, c, x)))
        break
    else:
        j_0 = delta.index(mindelta)
        Z = np.dot(B, A[:, j_0])
        theta = calc_steps(Z, x_b, m)
        theta_0 = min(theta)
    
        if theta_0 == INF:
            print('Задача не имеет решения - целевая ф-ция не ограничена сверху на мн-ве планов.')
            break
        else:
            s = theta.index(theta_0)
            x = calc_new_plan(x, J_b, J_n, j_0, Z, theta_0, m)
            J_b = calc_new_basis_indexes(J_b, j_0, s)
            J_n = nonbasis_indexes(J, J_b)
