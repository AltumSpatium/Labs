# lab 1, task 3


def storage():
    elements = set()
    arguments = []
    while True:
        arguments = raw_input(">").split()
        if arguments == []:
            continue
        elif arguments[0] == 'exit':
            break
        elif arguments[0] == 'add':
            for num in arguments[1:]:
                elements.add(num)
        elif arguments[0] == 'remove':
            for num in arguments[1:]:
                elements.discard(num)
        elif arguments[0] == 'find':
            for num in arguments[1:]:
                if num in elements:
                    print "Found:", num
        elif arguments[0] == 'list':
            print list(elements)
