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
            i += 1
    return A_b

def optimal(x, J_b):
    x_ = [0 for _ in J]
    for j in J_b:
        x_[j] = x[j]
    return x_

n = 8
m = 4
c = np.array([-6, -9, -5, 2, -6, 0, 1, 3], dtype=np.float)
A = np.array([[0, 1, 1, 1,   0,-8, 1,   5],
              [0,-1, 0,-7.5 ,0, 0, 0,   2],
              [0, 2, 1, 0,  -1, 3,-1.4, 0],
              [1, 1, 1, 1,   0, 3, 1,   1]], dtype=np.float)
b = np.array([15, -45, 1.8, 19], dtype=np.float)

x = np.array([4, 0, 6, 6, 0, 0, 3, 0], dtype=np.float)
J = [i for i in range(n)]

J_b = np.array([0, 2, 3, 6], dtype=np.int)
J_n = nonbasis_indexes(J, J_b)
A_b = np.array([[0, 1, 1, 1],
                [0, 0, -7.5, 0],
                [0, 1, 0, -1.4],
                [1, 1, 1, 1]], dtype=np.float)


# Task
# n = 8
# m = 4
# c = np.array([-5, -2, 3, -4, -6, 0, -1, -5], dtype=np.float)
# A = np.array([[0, 1, 4, 1, 0, -3, 5, 0],
#               [1, -1, 0, 1 ,0, 0, 1, 0],
#               [0, 7, -1, 0, -1, 3, 8, 0],
#               [1, 1, 1, 1, 0, 3, -3, 1]], dtype=np.float)
# b = np.array([6, 10, -2, 15], dtype=np.float)

# x = np.array([4, 0, 0, 6, 2, 0, 0, 5], dtype=np.float)
# J = [i for i in range(n)]

# J_b = np.array([0, 3, 4, 7], dtype=np.int)
# J_n = nonbasis_indexes(J, J_b)
# A_b = np.array([[0, 1, 0, 0],
#                 [1, 1, 0, 0],
#                 [0, 0, -1, 0],
#                 [1, 1, 0, 1]], dtype=np.float)

# n = 5
# m = 3
# c = np.array([5, 6, 0, 0, 0], dtype=np.float)
# A = np.array([[5, 9, 1, 0, 0],
#               [3, 3, 0 ,1 ,0],
#               [2, 1, 0, 0, 1]], dtype=np.float)
# b = np.array([45, 19, 10], dtype=np.float)

# x = np.array([0, 0, 45, 19, 10], dtype=np.float)
# J = [i for i in range(5)]

# J_b = np.array([2, 3, 4], dtype=np.int)
# J_n = nonbasis_indexes(J, J_b)
# A_b = np.array([[1, 0, 0],
#                 [0, 1, 0],
#                 [0, 0, 1]], dtype=np.float)
cnt = 1

while True:
    print('Итерация', cnt)
    x_b = basis_plan(x, J_b)
    c_b = basis_cost(c, J_b)
    B = inverse_basis_matrix(A_b)
    U = vector_of_potentials(c_b, B)
    delta = estimates(U, A, c, J_n)
    mindelta = min(delta)

    if (mindelta >= 0):
        print('Оптимальный план:')
        x_ = x
        print(x_)
        print('Прибыль: ', sum(map(lambda a, b: a*b, c, x_)))
        break
    else:
        j_0 = delta.index(mindelta)
        Z = np.dot(B, A[:, j_0])

        theta = calc_steps(Z, x_b, m)
        theta_0 = min(theta)
    
        if theta_0 == INF:
            print('Задача не имеет решения - целевая ф-ция не ограничена на мн-ве планов.')
            break
        else:
            s = theta.index(theta_0)
            j_s = J_b[s]

            x = calc_new_basis_plan(J, J_n, j_0, Z, theta_0, m)
            J_b = calc_new_basis_indexes(J_b, j_0, j_s)
            J_n = nonbasis_indexes(J, J_b)
            A_b = basis_matrix(A, A_b, J_b, m)
            cnt += 1
