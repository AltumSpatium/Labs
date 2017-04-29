import json
import numpy as np
from simplex import simplex

INPUT = 'task'


def convex(x, x_g, B_0, c_0, g_x):
    def f(x):
        return func(x, B_0, c_0, 0)


    def g(x, i):
        B, c, alpha = g_x[i]
        return func(x, B, c, alpha)


    I_0 = [i for i, _ in enumerate(g_x) if g(x, i) == 0]
    J_0 = [j for j, x in enumerate(x.flat) if x == 0]

    df_dx = B_0.transpose() * B_0 * x + c_0
    dg_dx = [B.transpose() * B * x + c for B, c, alpha in g_x]

    l_0 = linear(df_dx, dg_dx, I_0)
    df_dxl = df_dx.transpose() * l_0
    if df_dxl < 0:
        delta_x = x_g - x
        a = calc_a(df_dxl, df_dx, delta_x)
        t = 0.5
        x_t = x + t * (l_0  + a * delta_x)
        print('x(t):', list(x_t.flat))
        print('f(x(t)):', f(x_t)[0, 0])
        print('g(x(t)):')
        for i in range(len(g_x)):
            print(g(x_t, i)[0, 0])
    else:
        print('План оптимален')


def linear(df_dx, dg_dx, I_0):
    dg_dx_list = [list(dg_dx[i].flat) for i in range(len(dg_dx)) if i in I_0]

    A = np.array(dg_dx_list, dtype=float)
    x = np.array([-1, 0, 0.3136, -1, -1, -1, 0, 0], dtype=float)
    c = np.array([c for c in df_dx.flat], dtype=float)
    b = np.array([0], dtype=float)
    J_b = I_0
    return np.matrix([l for l in simplex(A, x, b, c, J_b)[1]]).transpose()


def func(x, B, c, alpha):
    return np.round(0.5 * x.transpose() * B.transpose() * B * x + c.transpose() * x + alpha, 3)


def calc_a(df_dxl, df_dx, delta_x):
    a = 1
    while True:
        if (df_dxl + a * (df_dx.transpose() * delta_x)) < 0:
            return a
        a += 1


def map_input(data):
    g_x = [(np.matrix(data['B_' + str(i)]),
            np.matrix(data['c_' + str(i)]).transpose(),
            data['alpha_' + str(i)]) for i in range(1, 6)]

    return (np.matrix(data['x']).transpose(),
            np.matrix(data['x_g']).transpose(),
            np.matrix(data['B_0']),
            np.matrix(data['c_0']).transpose(),
            g_x)


def main():
    with open(INPUT) as f:
        data = json.load(f)

    convex(*map_input(data))


if __name__ == '__main__':
    main()
