import json
from numpy import array
from rat import rat


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
    f = array(task['f'], dtype=float)
    return (f, )


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
            result = rat(task)

            if not result:
                print('Task doesn\'t have a solution!')
                return
            
            print()
            print('Resource allocation:', result[0])
            print('Bellman function table:\n', result[1])
            
            return


if __name__ == '__main__':
    main()
