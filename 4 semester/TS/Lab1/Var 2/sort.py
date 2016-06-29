# lab 1, task 2


def quicksort(lst):
    if len(lst) <= 1:
        return lst
    else:
        q = lst[len(lst) / 2]
        L, M, R = [], [], []
        for element in lst:
            if element < q:
                L.append(element)
            elif element > q:
                R.append(element)
            else:
                M.append(element)
        return quicksort(L) + M + quicksort(R)


def mergesort(lst):
    if len(lst) > 1:
        lst = merge(
            mergesort(lst[:(len(lst) / 2)]), mergesort(lst[(len(lst) / 2):]))
    return lst


def merge(left, right):
    lst = []
    while left and right:
        if left[0] < right[0]:
            lst.append(left.pop(0))
        else:
            lst.append(right.pop(0))
    if left:
        lst.extend(left)
    if right:
        lst.extend(right)
    return lst


def radixsort(lst):
    length = len(str(max(lst)))
    rng = 10
    for i in range(length):
        lists = [[] for _ in range(rng)]
        for num in lst:
            lists[(num / 10 ** i) % rng].append(num)
        lst = []
        for elem in lists:
            lst.extend(elem)
    return lst
