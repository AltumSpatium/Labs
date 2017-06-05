#! /usr/bin/env python3

import argparse
import json
from numpy import matrix, linspace, array, vstack, hstack, zeros, linalg
from scipy.integrate import odeint
import quad_programming
import matplotlib.pyplot as plt

EPSILON = 1e-15


def solve(t_0, t_end, N, A, D, b, c, x_start, H, g):
    step = (t_end - t_0) / N
    t_1 = t_0 + step
    u_bottom_limit, u_top_limit = -1, 1

    omega_t_end = odeint(lambda omega, t: list((A * matrix(omega).T).flat),
                         list(x_start.flat),
                         [t_0, t_end])[-1]

    # неправильный вектор из примера в методичке
    # omega_t_end = [2.2015, 2.5931, -1.8099]

    y_t_1 = odeint(lambda y, t: list(((A * matrix(y).T) + b).flat),
                   zeros(len(x_start)),
                   [t_0, t_1])[-1]

    z = odeint(lambda z, t: list((A * matrix(z).T).flat),
               y_t_1,
               linspace(t_1, t_end, N))

    B_0 = matrix(omega_t_end).T
    B = matrix(list(reversed(z))).T

    D_ = B.T * D * B
    A_ = H * B
    c_ = B.T * (c + D * B_0)
    b_ = g - H * B_0

    ensure_b_is_positive(A_, b_)
    A_, b_ = apply_u_bottom_limit(A_, b_, u_bottom_limit)
    ensure_b_is_positive(A_, b_)

    A_, b_, c_, D_ = apply_u_top_limit(A_, b_, c_, D_, u_top_limit - u_bottom_limit, N)

    J_op, J_star, start_plan = get_start_plan(A_, b_, u_top_limit - u_bottom_limit)
    if not J_op:
        print("Cannot find start plan for quad programming:(")
        return

    solution = quad_programming.solve(A_, b_, c_, D_, start_plan, J_op, J_star)

    show_plot(linspace(t_0, t_end, N),
              [u_j + u_bottom_limit for u_j in solution[0].flat][:N],
              u_bottom_limit,
              u_top_limit)


def show_plot(t_points, u, u_min, u_max):
    plt.plot(t_points, u, 'bo', label='u(t)')
    plt.legend(loc='best')
    plt.xlabel('t')
    plt.ylabel('u(t)')
    margin = 0.2
    plt.axis([t_points[0] - margin,
              t_points[-1] + margin,
              u_min - margin,
              u_max + margin])
    plt.grid()
    plt.show()


def ensure_b_is_positive(A_, b_):
    for i, value in enumerate(b_.flat):
        if value < 0:
            b_[i] *= -1
            A_[i] *= -1


def apply_u_bottom_limit(A_, b_, u_bottom_limit):
    for i, row in enumerate(A_):
        b_[i, 0] -= sum(u_bottom_limit * array(row.flat))
    return A_, b_


def apply_u_top_limit(A_, b_, c_, D_, rh_b, N):
    for j in range(N):
        size_i, size_j = A_.shape
        new_row = matrix(zeros(size_j + 1))
        new_row[0, j] = new_row[0, -1] = 1

        A_ = hstack((A_, matrix(zeros(size_i)).T))
        A_ = vstack((A_, new_row))
        b_ = vstack((b_, matrix([rh_b])))
        c_ = vstack((c_, matrix([0])))
        D_ = hstack((D_, matrix(zeros(len(D_))).T))
        D_ = vstack((D_, matrix(zeros(len(D_) + 1))))

    return A_, b_, c_, D_


def get_start_plan(A_, b_, top_limit):
    size_i, size_j = A_.shape

    def get_start_plan_for_basis(basis):
        try:
            basis_u = extract_columns(A_, basis) ** -1 * b_
        except linalg.linalg.LinAlgError:
            return [top_limit + 42] * len(basis)

        start_plan = zeros(size_j)
        for j, value in zip(basis, basis_u.flat):
            start_plan[j] = value
        return start_plan


    for basis_start_index in range(size_j - size_i + 1):
        basis = [j for j in range(basis_start_index, basis_start_index + size_i)]
        plan = get_start_plan_for_basis(basis)

        if all(-EPSILON < u_j < top_limit + EPSILON for u_j in plan):
            return basis, list(basis), matrix(plan).T

    return None, None, None


def extract_columns(matrix, indices):
    column_dimension = 1
    return matrix.take(indices, column_dimension)


def map_input(input_data):
    return (input_data['t_0'],
            input_data['t_end'],
            input_data['N'],
            matrix(input_data['A']),
            matrix(input_data['D']),
            matrix(input_data['b']).transpose(),
            matrix(input_data['c']).transpose(),
            matrix(input_data['x_start']).transpose(),
            matrix(input_data['H']),
            matrix(input_data['g']).transpose())


def _parse_args():
    parser = argparse.ArgumentParser(description='Optimal control problem solver.')
    parser.add_argument('input', type=str, help='Name of file with input')

    return parser.parse_args()


def main():
    args = _parse_args();

    with open(args.input) as input_file:
        input_data = json.load(input_file)

    solve(*map_input(input_data))


if __name__ == '__main__':
    main()
