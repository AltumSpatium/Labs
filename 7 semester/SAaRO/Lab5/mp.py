import numpy as np


def potentials_method(I, a, U_s, U_e, U_b, c, x):
    U = list(zip(U_s, U_e))

    if not check_balance(I, U, a, x):
        return None

    U_b = [tuple(U[i]) for i in U_b]

    while True:
        U_n = [arc for arc in U if arc not in U_b]
        u = calc_potentials(len(I), U, U_b, c)
        delta = calc_estimates(u, c, U, U_n)
        opt_criteria = check_optimal_criteria(delta, U)
        if opt_criteria[0]:
            return (x, sum(c * x), U)
    
        ij_0 = opt_criteria[1]
        U_c = build_cycle(U_b + [ij_0])
        U_cp, U_cm = split_cycle(U_c)
        if len(U_cm) == 0:
            return None

        theta, ij_s = calc_theta(U, U_cm, x)
        x = calc_new_flow(x, theta, U, U_cp)
        x = calc_new_flow(x, -theta, U, U_cm)
        U_b = calc_new_basis(U_b, ij_0, ij_s)


def check_balance(I, U, a, x):
    for i in I:
        I_p = []
        I_m = []
        for arc in [arc for arc in U if i in arc]:
            if i == arc[0]:
                I_p.append(arc)
            elif i == arc[1]:
                I_m.append(arc)
        res = sum([x[U.index(arc)] for arc in I_p]) - sum([x[U.index(arc)] for arc in I_m])
        if a[i-1] != res:
            return False
    return True


def calc_potentials(n, U, U_b, c):
    u = [0] + [None] * (n - 1)
    while True:
        for arc in U_b:
            i, j = arc[0] - 1, arc[1] - 1
            if u[i] is not None and u[j] is None:
                u[j] = u[i] - c[U.index(arc)]
            elif u[j] is not None and u[i] is None:
                u[i] = c[U.index(arc)] + u[j]
        if not None in u:
            break
    return u


def calc_estimates(u, c, U, U_n):
    delta = [None] * len(U)
    for arc in U_n:
        i, j = arc[0] - 1, arc[1] - 1
        delta[U.index(arc)] = u[i] - u[j] - c[U.index(arc)]
    return delta


def check_optimal_criteria(delta, U):
    for estimate in delta:
        if estimate is not None and estimate > 0:
            return False, U[delta.index(estimate)]
    return True, None


def build_cycle(U_bc):
    result = U_bc[:]

    while True:
        found = False
        for arc in U_bc:
            U_sc = [arc[0] for arc in result]
            U_ec = [arc[1] for arc in result]
            i, j = arc
            if (U_sc + U_ec).count(i) == 1 or (U_sc + U_ec).count(j) == 1:
                if arc in result:
                    result.remove(arc)
                found = True
        if not found:
            break
    
    return result


def split_cycle(U_c):
    U_cp = [U_c[-1]]
    del U_c[-1]
    U_c_ = U_c[:]

    while True:
        found = False
        for arc in U_c_:
            for arc_cp in U_cp[:]:
                if arc[0] == arc_cp[1] or arc[1] == arc_cp[0]:
                    U_cp.append(arc)
                    U_c_.remove(arc)
                    found = True
        if not found:
            break
    U_cm = [arc for arc in U_c if arc not in U_cp]
    return U_cp, U_cm


def calc_theta(U, U_cm, x):
    result = [x[U.index(arc)] for arc in U_cm]
    theta = min(result)
    return theta, U_cm[result.index(theta)]


def calc_new_flow(x, theta, U, U_c_):
    for arc in U_c_:
        x[U.index(arc)] += theta
    return x


def calc_new_basis(U_b, ij_0, ij_s):
    return U_b[:U_b.index(ij_s)] + [ij_0] + U_b[U_b.index(ij_s) + 1:]
