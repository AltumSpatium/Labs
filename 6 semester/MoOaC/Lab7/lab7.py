#! /usr/bin/env python3

import argparse
import json
from sympy.parsing.sympy_parser import parse_expr
from sympy import symbols, Matrix
from sympy.solvers import solve
from pprint import pprint
from numpy import isclose


def check(variables, f, limitations, x0):
    substitution_dict = { var:value for var, value in zip(variables, x0) }
    lambdas = symbols('lamda0:{}'.format(len(limitations) + 1))
    ls = symbols('l0:{}'.format(len(x0) + 1))

    for limitation in limitations:
        if not limitation.subs(substitution_dict):
            print('Данный ветктор не является планом')
            return

    active_limitations = [i for i, limitation in enumerate(limitations)
                          if isclose(float(limitation.lhs.subs(substitution_dict).evalf()),
                                     float(limitation.rhs.evalf()))]

    fx0_dx = [f.diff(var).subs(substitution_dict).evalf() for var in variables]
    gx0_dx = [[g.lhs.diff(var).subs(substitution_dict).evalf() for var in variables]
              for g in limitations]

    print('Активные ограничения dg(x0) / dx:')
    pprint([vector for i, vector in enumerate(gx0_dx) if i in active_limitations])
    print()
    print('Индексы активных ограничений:')
    pprint(active_limitations)
    print()

    equations = []
    for i, fx0_dxi in enumerate(fx0_dx):
        for j, gx0_j in enumerate(gx0_dx):
            if j in active_limitations:
                fx0_dxi += lambdas[j] * gx0_j[i]
        equations.append(fx0_dxi)

    print()
    print('Система уравнений:')
    for equation in equations:
        print(equation, '= 0')
    print()

    solution = solve(equations)

    if not solution:
        print ("Нет решения")
        return

    lamda = [0] * len(limitations)
    for i, var in enumerate(lambdas):
        if var in solution:
            lamda[i] = solution[var]
            if lamda[i] < 0 and i in active_limitations:
                print(lamda[i], '< 0')
                print('Не оптимален')
                #return

    print('Решение:')
    print(lamda)
    print()

    F = f
    for g, lam in zip(limitations, lamda):
        F += lam * g.lhs

    d2F_dx2 = []
    for i in range(len(variables)):
        row = []
        for j in range(len(variables)):
            row.append(F.diff(variables[i], variables[j])
                        .subs(substitution_dict)
                        .evalf())
        d2F_dx2.append(row)

    print('Матрица вторых производных d2L(x, lambda) / dx2:')
    pprint(Matrix(d2F_dx2))

    equations = []
    for i, gx0 in enumerate([vector for i, vector in enumerate(gx0_dx)
                             if i in active_limitations]):
        eq = 0
        for j, gx0_j in enumerate(gx0):
            eq += ls[j] * gx0_j
        equations.append(eq)

    solution = solve(equations)
    l = Matrix([solution[var] for var in ls if var in solution])
    print()
    print('Решение l:', l)
    print()

    if (l.T * Matrix(d2F_dx2) * l)[0, 0] >= 0:
        print('План локально отимален')
    else:
        print('Не оптимален')


def parse_input(input_data):
    variables = symbols(input_data['vars'])
    f = parse_expr(input_data['f'])
    limitations = [parse_expr(expr) for expr in input_data['limitations_ineq']]
    x0 = input_data['x0']

    return variables, f, limitations, x0


def _parse_args():
    parser = argparse.ArgumentParser(description='Nonlinear programming plan checker.')
    parser.add_argument('input', type=str, help='Name of file with input')

    return parser.parse_args()


def main():
    args = _parse_args();

    with open(args.input) as input_file:
        input_data = json.load(input_file)

    check(*parse_input(input_data))


if __name__ == '__main__':
    main()
