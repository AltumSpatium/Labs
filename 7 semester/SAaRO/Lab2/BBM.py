import math
from DSM import DSM

INF = float('inf')


class TaskList(object):
    def __init__(self, initial_tasks=[]):
        if not isinstance(initial_tasks, list):
            initial_tasks = []
        self.tasks = initial_tasks


    def add(self, task):
        if not isinstance(task, Task):
            raise TypeError('Argument must be a Task object!')

        self.tasks.append(task)


    def get_next(self):
        return self.tasks.pop(0)


    def __str__(self):
        return str(self.tasks)


    def __len__(self):
        return len(self.tasks)


class Task(object):
    tasks_count = 0


    def __init__(self, A, b, c, d_l, d_h, J_b):
        self.A = A
        self.b = b
        self.c = c
        self.d_l = d_l
        self.d_h = d_h
        self.J_b = J_b

        Task.tasks_count += 1
        self.number = Task.tasks_count


    def get_task(self):
        return (self.A.copy(), self.b.copy(), self.c.copy(),
            self.d_l.copy(), self.d_h.copy(), self.J_b.copy())


    def calc(self):
        return DSM(self.A, self.b, self.c, self.d_l, self.d_h, self.J_b)


    def __repr__(self):
        return 'Task<{0}>'.format(self.number)


def test_result(result):
    for i in range(len(result[0])):
        x = result[0][i]
        if math.modf(x)[0] != 0:
            return i
    return -1


def split_task(task, result, float_index):
    task1 = Task(*task.get_task())
    task2 = Task(*task.get_task())

    task1.d_l[float_index] = math.floor(result[0][float_index]) + 1
    task2.d_h[float_index] = math.floor(result[0][float_index])
    return task1, task2


def BBM(task):
    r = -INF
    task_list = TaskList([Task(*task)])
    result_list = []

    while True:
        if len(task_list) == 0:
            return result_list if len(result_list) != 0 else None

        next_task = task_list.get_next()
        result = next_task.calc()
        if not result or result[1] < r:
            continue

        float_index = test_result(result)

        if float_index != -1:
            task1, task2 = split_task(next_task, result, float_index)
            task_list.add(task1)
            task_list.add(task2)
        else:
            result_list.append(result)
            r = result[1]
