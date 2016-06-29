# lab 1, task 4


def fib(n):
    a, b = 0, 1
    while a < n:
        yield a
        a, b = b, a+b


def print_fib(n):
    for i in fib(n):
        print i
