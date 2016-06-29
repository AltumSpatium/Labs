import sys
import argparse
import text
import sort
import storage
import fib


def createParser():
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--file', default='text.txt')
    parser.add_argument('-t', '--task', type=int)
    parser.add_argument('-s', '--subtask', type=int)
    return parser


def main():
    parser = createParser()
    argument = parser.parse_args(sys.argv[1:])
    try:
        if argument.task == 1:
            if argument.subtask == 1:
                with open(argument.file, 'r') as file:
                    text.repeats(file)
            elif argument.subtask == 2:
                with open(argument.file, 'r') as file:
                    text.average(file)
            elif argument.subtask == 3:
                with open(argument.file, 'r') as file:
                    text.median(file)
            elif argument.subtask == 4:
                with open(argument.file, 'r') as file:
                    text.top(file)
        elif argument.task == 2:
            if argument.subtask == 1:
                lst = raw_input("Enter list: ").split()
                lst = [int(elem) for elem in lst]
                print sort.quicksort(lst)
            elif argument.subtask == 2:
                lst = raw_input("Enter list: ").split()
                lst = [int(elem) for elem in lst]
                print sort.mergesort(lst)
            elif argument.subtask == 3:
                lst = raw_input("Enter list: ").split()
                lst = [int(elem) for elem in lst]
                print sort.radixsort(lst)
        elif argument.task == 3:
            storage.storage()
        elif argument.task == 4:
            n = int(raw_input("Enter n: "))
            fib.print_fib(n)
    except Exception as e:
        print e

if __name__ == "__main__":
    sys.exit(main())
