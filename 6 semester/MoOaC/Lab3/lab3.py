import argparse
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

def dual_simplex_method():
    global A, b, c, J, J_b, J_n, n, m, x, y
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
            sigma_0 = np.amin(sigma)
            if sigma_0 == INF:
                print("Задача не имеет решения")
                break
            else:
                j_s = J_n[sigma.index(sigma_0)]
                y = calc_new_dual_plan(y, sigma_0, delta_y)
                J_b = calc_new_basis_indexes(J_b, k, j_s)
                J_n = nonbasis_indexes(J, J_b)   

n = 8
m = 3
J = [i for i in range(n)]
c = np.array([-5, -2, 3, -4, -6, 0, -1, -5], dtype=float)
b = np.array([6, 10, -2], dtype = float)
A = np.array([[0, 1, 4, 1, 0, -3, 5, 0], 
              [1, -1, 0, 1, 0, 0, 1, 0],
              [0, 7, -1, 0, -1, 3, 8, 0]], dtype=float)
x = np.zeros(n, dtype=float)

J_b = [0, 2, 4]
J_n = nonbasis_indexes(J, J_b)

y = np.array([2.25, -5, 6], dtype=float)

limitations = [
    ([1, 1, 1, 1, 0, 3, -3, 1, 1], 9)
]

def format_limitation(limitation):
    result = ""
    for i in range(len(limitation[0])):
        x = limitation[0][i]
        sign = "" if i == 0 else (" + " if x >=0 else " - ")
        result += "{0}{1}*x{2}".format(sign, abs(x), i + 1)
    return result + " = " + str(limitation[1])

def resize_array(arr, val):
    arr_list = arr.tolist()
    arr_list.append(val)
    return np.array(arr_list, dtype=float)

def add_limitation(limitation):
    global n, m, J, c, b, A, x, J_b, J_n, y
    n += 1
    m += 1
    J.append(n-1)
    c = resize_array(c, 0)
    b = resize_array(b, limitation[1])
    x = resize_array(x, 0)
    y = resize_array(y, 0)
    J_b.append(n-1)
    J_n = nonbasis_indexes(J, J_b)
    A_list = A.tolist()
    for row in A_list: row.append(0)
    A_list.append(limitation[0])
    A = np.array(A_list, dtype=float)

def main():
    parser = argparse.ArgumentParser()
    group = parser.add_mutually_exclusive_group()
    group.add_argument('-c', help='if condition vector was changed', action='store_true')
    group.add_argument('-s', help='if size of the task was changed', action='store_true')
    args = parser.parse_args()

    if args.s:
        print(y)
        for limitation in limitations:
            print('+ ограничение:', format_limitation(limitation))
            add_limitation(limitation)
            dual_simplex_method()
        print(y)
    elif args.c:
        pass

if __name__ == '__main__':
    main()
