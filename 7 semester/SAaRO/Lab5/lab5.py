import json
from numpy import array
from mp import potentials_method


def load_tasks():
    filename = 'tasks.json'
    try:
        with open(filename) as f:
            tasks = f.read()
        return json.loads(tasks)
    except Exception:
        print('File tasks.json does not exist!')
        return None


def parse_task(task):
    I = array(list(range(1, task['I'] + 1)))
    a = array(task['a'])
    U_s = array(task['U_s'])
    U_e = array(task['U_e'])
    U_b = array(task['U_b'])
    c = array(task['c'])
    x = array(task['x'])
    return (I, a, U_s, U_e, U_b, c, x)


def main():
    tasks = load_tasks()
    if not tasks:
        return
    while True:
        num = int(input('Enter task number: '))
        if num >= len(tasks):
            print('Incorrect task number!')
            continue
        else:
            task = parse_task(tasks[num])
            result = potentials_method(*task)

            if not result:
                print('Task doesn\'t have a solution!')
                return
            
            print()
            print('Optimal flow:')
            for i in range(len(result[0])):
                x = result[0][i]
                arc = result[2][i]
                print('X{0},{1} = {2}'.format(arc[0], arc[1], x))
            print('Target function value:', result[1])
            
            return


if __name__ == '__main__':
    main()
