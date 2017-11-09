def labels_method(I, U_s, U_e, c):
    U = list(zip(U_s, U_e))
    I_m = [1]
    labels = [(0, 0)] + [None] * (len(I) - 1)

    while False in [i in I_m for i in I]:
        w = get_neighbour_nodes(I, I_m, U)
        labels_temp = []
        for node in w:
            U_w = [(i, node) for i in I_m if (i, node) in U]
            B = [labels[arc[0]-1][0] + c[U.index(arc)] for arc in U_w]
            B_t = min(B)
            f_t = U_w[B.index(B_t)][0]
            labels_temp.append((B_t, f_t))
        B_temp = list(map(lambda label: label[0], labels_temp))
        B_j = min(B_temp)
        node_index = B_temp.index(B_j)
        labels[w[node_index]-1] = (B_j, labels_temp[node_index][1])
        I_m.append(w[node_index])
    return list(map(lambda x: x[0], labels))


def get_neighbour_nodes(I, I_m, U):
    w = set()
    for node in I_m:
        for arc in U:
            if node == arc[0] and arc[1] not in I_m:
                w.add(arc[1])
    return list(w)
