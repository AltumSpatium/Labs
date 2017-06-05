#! /usr/bin/env python3

import argparse
import json
from numpy import matrix, vstack, hstack, zeros
from math import inf

EPSILON = 1e-15


def solve(A, b, c, D, x, J_op, J_star):
    gradient = D * x + c
    A_op = extract_columns(A, J_op)
    c_op = extract_rows(gradient, J_op)

    # потенциалы
    u = (-c_op.transpose() * A_op ** -1).transpose()
    # оценки
    delta = u.transpose() * A + gradient.transpose()

    is_optimal, j_0 = is_optimal_plan(delta, J_star)
    if is_optimal:
        print('Win')
        return x, c.transpose() * x + 0.5 * x.transpose() * D * x

    while True:
        l = get_directions(J_star, j_0, len(x.flat), A, D)

        min_theta, min_j = get_min_step(J_star, j_0, l, D, x, delta[0, j_0])

        if min_theta == inf:
            print('No solutions')
            return

        x = x + min_theta * l

        J_op, J_star, is_next_iteration = get_new_basis(J_op,
                                                        J_star,
                                                        j_0,
                                                        min_j,
                                                        A_op,
                                                        A)
        delta[0, j_0] += min_theta * (l.transpose() * D * l)
        if is_next_iteration:
            break

    return solve(A, b, c, D, x, J_op, J_star)


def is_optimal_plan(delta, J_star):
    for j, mark in enumerate(delta.flat):
        if j not in J_star and mark < -EPSILON:
            return (False, j)

    return (True, None)


def get_directions(J_star, j_0, basis_size, A, D):
    l = [0] * basis_size
    l[j_0] = 1

    D_star = extract_columns(extract_rows(D, J_star), J_star)
    A_star = extract_columns(A, J_star)
    D_star_j_0 = extract_columns(extract_rows(D, J_star), [j_0])
    A_j_0 = extract_columns(A, [j_0])

    zero_matrix_size = min(A_star.shape)
    H = vstack((hstack((D_star, A_star.transpose())),
                hstack((A_star, zeros((zero_matrix_size, zero_matrix_size))))
               ))
    bb = vstack((D_star_j_0, A_j_0))

    for j, direction in zip(J_star, (-H ** -1 * bb).flat):
        l[j] = direction

    return matrix(l).transpose()


def get_min_step(J_star, j_0, l, D, x, delta_j_0):
    theta_J_star = []
    flat_l, flat_x = list(l.flat), list(x.flat)
    for j in J_star:
        value_to_append = inf if flat_l[j] >= -EPSILON else -flat_x[j] / flat_l[j]
        theta_J_star.append(value_to_append)

    l_D_l = l.transpose() * D * l

    theta_j_0 = inf
    if l_D_l > EPSILON:
        theta_j_0 = abs(delta_j_0) / float(l_D_l)
    if l_D_l < -EPSILON:
        raise Exception('wtf')

    min_theta = min(min(theta_J_star), theta_j_0)
    min_index = j_0 if min_theta == theta_j_0 else J_star[theta_J_star.index(min_theta)]

    return min_theta, min_index


def get_new_basis(J_op, J_star, j_0, j_min, A_op, A):
    J_star_set, J_op_set = set(J_star), set(J_op)
    set_diff = J_star_set - J_op_set
    j_plus, s = None, None
    is_new_iteration = True

    if j_min in J_star:
        s = J_star.index(j_min)

    for j in set_diff:
        e_s = matrix(zeros(len(J_op))).transpose()
        e_s[s, 0] = 1
        if e_s.transpose() * A_op ** -1 * extract_columns(A, [j]) != 0:
            j_plus = j
            break

    if j_0 == j_min:
        J_star_set.add(j_0)

    elif j_min in set_diff:
        J_star_set.discard(j_min)
        is_new_iteration = False

    elif j_min in J_op and j_plus is not None:
        J_star_set.discard(j_min)
        J_op_set.discard(j_min)
        J_op_set.add(j_plus)
        is_new_iteration = False

    else:
        J_star_set.discard(j_min)
        J_star_set.add(j_0)
        J_op_set.discard(j_min)
        J_op_set.add(j_0)

    return list(sorted(J_op_set)), list(sorted(J_star_set)), is_new_iteration


def extract_columns(matrix, indices):
    column_dimension = 1
    return matrix.take(indices, column_dimension)


def extract_rows(matrix, indices):
    row_dimension = 0
    return matrix.take(indices, row_dimension)


def map_input_to_numpy_matrices(input_data):

    def get_D():
        if 'D' in input_data:
            return matrix(input_data['D'])
        else:
            B = matrix(input_data['B'])
            return B.transpose() * B

    def get_c():
        if 'c' in input_data:
            return matrix(input_data['c']).transpose()
        else:
            return (-1 * matrix(input_data['d']) * matrix(input_data['B'])).transpose()

    def get_basis_indices(basis_name):
        return [index - 1 for index in input_data[basis_name]]

    return (matrix(input_data['A']),
            matrix(input_data['b']).transpose(),
            get_c(),
            get_D(),
            matrix(input_data['x_start']).transpose(),
            get_basis_indices('J_op'),
            get_basis_indices('J_star'))


def _parse_args():
    parser = argparse.ArgumentParser(description='Quad programming.')
    parser.add_argument('input', type=str, help='Name of file with input')

    return parser.parse_args()


def main():
    args = _parse_args();

    with open(args.input) as input_file:
        input_data = json.load(input_file)

    solution = solve(*map_input_to_numpy_matrices(input_data))
    print(solution)


if __name__ == '__main__':
    main()
