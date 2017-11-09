import json
from labels_method import labels_method


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
    I = task['I']
    I = list(range(1, task['I'] + 1))
    U_s = task['U_s']
    U_e = task['U_e']
    c = task['c']
    return (I, U_s, U_e, c)


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
            result = labels_method(*task)

            if not result:
                print('Task doesn\'t have a solution!')
                return
            
            print()
            print('Lengths of shortest paths:')
            for i in range(len(result)):
                B = result[i]
                print('B{0} = {1}'.format(i+1, B))
            
            return


if __name__ == '__main__':
    main()
