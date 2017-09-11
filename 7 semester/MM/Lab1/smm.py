import random
import time
import math


def smm(iv, rank=4):
    def extend_value(val):
        return '0' * (rank * 2 - len(val)) + val

    square_val = str(iv ** 2)
    square_val = square_val if len(square_val) == rank * 2 else extend_value(square_val)
    return int(square_val[rank//2 : -rank//2])


def smm_generator(iv, rank=4):
    if len(str(iv)) != rank:
        raise ValueError('Initial number must have a rank of ' + str(rank) + '!')

    next_val = smm(iv, rank=rank)
    while True:
        yield next_val
        next_val = smm(next_val, rank=rank)


def smm_random(rank=4):
    random.seed(time.time())
    iv = math.floor(random.random() * 10 ** rank)
    return smm_generator(iv, rank=rank)
