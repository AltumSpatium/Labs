import json
from numpy import array
from DSM import DSM


def load_tasks():
    filename = 'tasks.json'
    try:
        with open(filename) as f:
            tasks = f.read()
        return json.loads(tasks)
    except Exception:
        print('tasks.json is not exists!')
        return None


def parse_task(task):
    A = array(task['A'], dtype=float)
    b = array(task['b'], dtype=float)
    c = array(task['c'], dtype=float)
    d_l = array(task['d_l'], dtype=float)
    d_h = array(task['d_h'], dtype=float)
    J_b = task['J_b']
    return (A, b, c, d_l, d_h, J_b)


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
            result = DSM(*task)

            if not result:
                print('Task doesn\'t have a solution!')
                return
            
            print()
            print('Optimal plan:', result[0])
            print('Target function value:', result[1])
            
            return


if __name__ == '__main__':
    main()
