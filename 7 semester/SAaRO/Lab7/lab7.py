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
    t = task['t']
    I = task['I']
    I = list(range(1, task['I'] + 1))
    U_s = task['U_s']
    U_e = task['U_e']
    c = task['c']
    return (t, I, U_s, U_e, c)


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
            print('Longest path length:', result[0])
            print('Path:', result[1])
            
            return


if __name__ == '__main__':
    main()
