from collections import namedtuple, defaultdict
from functools import reduce

import gurobipy as g
import sys

Task = namedtuple('Task', 's d b c')

if __name__ == "__main__":
    assert len(sys.argv) >= 3

    model = g.Model()

    # we add variables on the fly
    with open(sys.argv[1], "r", encoding="UTF-8") as file_in:
        n_customers, n_bureaus = map(int, file_in.readline().split())
        customers = {}
        bureaus = defaultdict(list)
        tasks = []

        for customer in range(n_customers):
            row = list(map(int, file_in.readline().split()))
            current_tasks = [Task(model.addVar(vtype=g.GRB.INTEGER, lb=0), d, b, customer) for b, d
                             in zip(row[::2], row[1::2])]
            tasks.extend(current_tasks)
            customers[customer] = tasks
            for task in current_tasks:
                bureaus[task.b].append(task)

    # constraints
    best_time = model.addVar(vtype=g.GRB.INTEGER, lb=0, obj=1)

    # finish all tasks before best_time
    for task in tasks:
        model.addConstr(task.s + task.d <= best_time)

    # doesn't start next task before the previous finishes
    for customers_tasks in customers.values():
        for t in range(len(customers_tasks) - 1):
            model.addConstr(customers_tasks[t].s + customers_tasks[t].d <= customers_tasks[t + 1].s)

    # no overlapping tasks for each bureau
    M = reduce(lambda x, t: x + t.d, tasks, 0)

    for bureaus_tasks in bureaus.values():
        for i in range(len(bureaus_tasks)):
            for j in range(i + 1, len(bureaus_tasks)):
                k = model.addVar(vtype=g.GRB.BINARY)
                model.addConstr(bureaus_tasks[i].s + bureaus_tasks[i].d <= bureaus_tasks[j].s + k * M)
                model.addConstr(bureaus_tasks[j].s + bureaus_tasks[j].d <= bureaus_tasks[i].s + (1 - k) * M)

    model.optimize()

    with open(sys.argv[2], "w+", encoding="UTF-8") as file_out:
        file_out.write(f"{int(model.getObjective().getValue())}\n")
        for bureaus_tasks in bureaus.values():
            file_out.write(f"{' '.join([str(task.c) for task in sorted(bureaus_tasks, key=lambda t: t.s.X)])}\n")
