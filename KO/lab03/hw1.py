#!/usr/bin/env python3
import sys

import gurobipy as g  # import Gurobi module

if __name__ == "__main__":
    with open(sys.argv[1], mode='r') as f:
        demands = list(map(int, f.read().split()))

    model = g.Model()

    z = [model.addVar(lb=0, vtype=g.GRB.INTEGER, name='slack{}'.format(i)) for i in range(len(demands))]
    shifts = [model.addVar(lb=0, vtype=g.GRB.INTEGER, name='shift{}'.format(i)) for i in range(len(demands))]

    model.update()

    for i in range(len(demands)):
        model.addConstr(demands[i] - g.quicksum([shifts[j % len(demands)] for j in range(i - 7, i + 1)]) <= z[i])
        model.addConstr(g.quicksum([shifts[j % len(demands)] for j in range(i - 7, i + 1)]) - demands[i] <= z[i])

    model.setObjective(g.quicksum(z), sense=g.GRB.MINIMIZE)
    model.optimize()

    with open(sys.argv[2], mode='w') as f:
        f.write(f'{int(model.objVal)}\n')
        f.write(f'{" ".join(map(str, map(int, map(lambda var: var.x, shifts))))}\n')
