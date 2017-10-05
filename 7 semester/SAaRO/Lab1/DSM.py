import numpy as np
import numpy.linalg as la

INF = float('inf')

def DSM(A, b, c, d_l, d_h, J_b):
    J = [i for i in range(A.shape[1])]
    
    c_b = c[J_b]
    A_b = A[:, J_b]
    B = la.inv(A_b)
    y = c_b.dot(B)
    coplan = y.dot(A) - c

    while True:
        J_n = [j for j in J if j not in J_b]
        J_np = [j for j in J_n if coplan[j] >= 0]
        J_nm = [j for j in J_n if j not in J_np]

        kappa = __calculate_kappa(d_l, d_h, J, J_b, J_n, J_np, J_nm, B, b, A)
        if __test_optimal_criteria(kappa, d_l, d_h, J_b):
            return (kappa, sum(c * kappa))
        
        j_k = [j for j in J_b if kappa[j] < d_l[j] or kappa[j] > d_h[j]][0]
        k = J_b.index(j_k)

        mu_jk = 1 if kappa[j_k] < d_l[j_k] else -1

        e_k = np.array([0] * A.shape[0])
        e_k[k] = 1

        d_y = mu_jk * e_k.dot(B)
        mu = d_y.dot(A)

        sigma = __calculate_sigma(coplan, mu, J_n, J_np, J_nm)
        sigma_0 = min(sigma)
        j_s = sigma.argmin()

        if sigma_0 == INF:
            return None

        coplan = coplan + sigma_0 * mu

        J_b[J_b.index(j_k)] = j_s
        
        A_b = A[:, J_b]
        B = la.inv(A_b)


def __calculate_kappa(d_l, d_h, J, J_b, J_n, J_np, J_nm, B, b, A):
    kappa = np.zeros(len(J))
    kappa[J_np] = d_l[J_np]
    kappa[J_nm] = d_h[J_nm]
    kappa[J_b] = B.dot(b - sum([A[:, j] * kappa[j] for j in J_n]))
    return kappa


def __test_optimal_criteria(kappa, d_l, d_h, J_b):
    left = d_l[J_b] <= kappa[J_b]
    right = kappa[J_b] <= d_h[J_b]
    if False in left or False in right:
        return False
    return True


def __calculate_sigma(coplan, mu, J_n, J_np, J_nm):
    sigma = np.zeros(len(coplan))
    sigma.fill(INF)
    for j in J_n:
        if (j in J_np and mu[j] < 0) or (j in J_nm and mu[j] > 0):
            sigma[j] = -coplan[j]/mu[j]
    return sigma
