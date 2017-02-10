import numpy as np
import numpy.linalg as la

INF = float('inf')

def basis_cost(c, J_b):
    c_ = []
    for j in J_b:
        if j > len(c) - 1:
            c_.append(0)
        else:
            c_.append(c[j])
    return np.array(c_, dtype=np.float)

def basis_plan(x, J_b):
    x_ = [x[j] for j in J_b]
    return np.array(x_, dtype=np.float)

def vector_of_potentials(c_b, B):
    return np.dot(c_b, B)

def estimates(U, A, c, J_n):
    return [np.dot(U, A[:, j]) - c[j] for j in J_n]

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

def calc_new_basis_indexes(J_b, j_0, j_s):
    J_ = []
    for j in J_b:
        if j == j_s:
            J_.append(j_0)
        else:
            J_.append(j)
    return np.array(J_, dtype=np.int)

def calc_new_basis_plan(J, J_n, j_0, Z, theta_0, m):
    x_ = []
    i = 0
    for j in J:
        if j == j_0:
            x_.append(theta_0)
        elif j in J_n:
            x_.append(0)
        else:
            x_.append(x_b[i] - theta_0 * Z[i])
            if i < m:
                i += 1
    return np.array(x_, dtype=np.float)

def nonbasis_indexes(J, J_b):
    return [j for j in J if j not in J_b]

def basis_matrix(A, A_b, J_b, m):
    i = 0
    for j in J_b:
        A_b[:, i] = A[:, j]
        if i < m:
            i+= 1
    return A_b

# Task
n = 5
m = 3
c = np.array([5, 6], dtype=np.float)
A = np.array([[5, 9, 1, 0, 0],
              [3, 3, 0 ,1 ,0],
              [2, 1, 0, 0, 1]], dtype=np.float)
b = np.array([45, 19, 10], dtype=np.float)

x = np.array([0, 0, 45, 19, 10], dtype=np.float)
J = [i for i in range(5)]

J_b = np.array([2, 3, 4], dtype=np.int)
J_n = nonbasis_indexes(J, J_b)
A_b = np.array([[1, 0, 0],
                [0, 1, 0],
                [0, 0, 1]], dtype=np.float)

#x_b = np.array([45, 19, 10], dtype=np.float)
#c_b = np.array([0, 0, 0], dtype=np.float)
x_b = basis_plan(x, J_b)
c_b = basis_cost(c, J_b)

B = inverse_basis_matrix(A_b)

U = vector_of_potentials(c_b, B)
delta = estimates(U, A, c, J_n)
mindelta = min(delta)

if (mindelta >= 0):
    print('Оптимальный план:')
    print(x_b)
else:
    j_0 = delta.index(mindelta)
    Z = np.dot(B, A[:, j_0])

    theta = calc_steps(Z, x_b, m)
    theta_0 = min(theta)
    
    if theta_0 == INF:
        print('Нет решений')
    else:
        s = theta.index(theta_0)
        j_s = J_b[s]

        x_b = calc_new_basis_plan(J, J_n, j_0, Z, theta_0, m)
        J_b = calc_new_basis_indexes(J_b, j_0, j_s)
        J_n = nonbasis_indexes(J, J_b)
        c_b = basis_cost(c, J_b)
        A_b = basis_matrix(A, A_b, J_b, m)