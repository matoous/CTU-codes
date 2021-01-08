# Problem description: a group of people share expences for an event. 
# Afterwards, they want to decide on the transactions between the people 
# such that each person paid an equal amount and the number of transactions is minimal.

import gurobipy as g  # import Gurobi module

# data ----------------------------------------------------
P = set(["A", "B", "N", "J"])
c = {"A": 0, "B": 590, "N": 110, "J": 300}

# model --------------------------------------------------
m = g.Model()

# - add variables
f = {}
y = {}
for p in P:
    for q in P:
        f[p, q] = m.addVar(lb=0, vtype=g.GRB.CONTINUOUS)
        y[p, q] = m.addVar(vtype=g.GRB.BINARY)

# - add constraints
s = sum(c.values()) / len(c)
for p in P:
    m.addConstr(g.quicksum(f[p, q] for q in P) + c[p] - g.quicksum(f[q, p] for q in P) == s)

M = sum(c.values())
#M = 1000000000
for p in P:
    for q in P:
        m.addConstr(f[p, q] <= y[p, q] * M)
        m.addConstr(y[p, q] <= f[p, q])

# - set objective
m.setObjective(g.quicksum(y[p, q] for p in P for q in P), sense=g.GRB.MINIMIZE)

# call the solver -------------------------------------------
m.optimize()

# print the solution -----------------------------------------
print('solution:')
for p in P:
    for q in P:
        print("{0} -> {1}: {2}".format(p, q, f[p, q].x))

print([y[p, q].x for p in P for q in P])

# always round results before casting to int!
print([int(round(f[p, q].x)) for p in P for q in P])

