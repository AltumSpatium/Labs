def labels_method(t, I, U_s, U_e, c):
    U = list(zip(U_s, U_e))
    I_m = [1]
    labels = [(0, 0)] + [None] * (len(I) - 1)
    
    while not t in I_m:
        w = get_neighbour_nodes(I, I_m, U)
        for node in w:
            I_s_ = [i for i in I if (i, node) in U]
            if not False in [node in I_m for node in I_s_]:
                B = [labels[i-1][0] + c[U.index((i, node))] for i in I_s_]
                B_j = max(B)
                f = I_s_[B.index(B_j)]
                labels[node-1] = (B_j, f)
                I_m.append(node)

    path = get_path(t, labels)
    return (labels[t-1][0], path)


def get_neighbour_nodes(I, I_m, U):
    w = set()
    for node in I_m:
        for arc in U:
            if node == arc[0]:
                w.add(arc[1])
    return list(w)


def get_path(t, labels):
    f = labels[t - 1][1]
    path = str(t)
    while f != 0:
        path += str(f)
        f = labels[f - 1][1]
    return ' -> '.join(reversed(path))
