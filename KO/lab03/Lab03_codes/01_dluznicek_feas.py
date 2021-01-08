# Problem description: a group of people share expences for an event. 
# Afterwards, they want to decide on the transactions between the people 
# such that each person paid an equal amount.

import gurobipy as g  # import Gurobi module

# data ------------------------------------------------------
P = set(["A", "B", "N", "J"])
c = {"A": 0, "B": 590, "N": 110, "J": 300}

# model -----------------------------------------------------
m = g.Model()

# - add variables
f = dict()
for p in P:
    for q in P:
        f[p, q] = m.addVar(lb=0, vtype=g.GRB.CONTINUOUS)

# - add constraints
s = sum(c.values()) / len(c)		
for p in P:
    m.addConstr(g.quicksum(f[p, q] for q in P) + c[p] - g.quicksum(f[q, p] for q in P) == s)

# - set objective
m.setObjective(0, sense=g.GRB.MINIMIZE)

# call the solver -------------------------------------------
m.optimize()

# print the solution ----------------------------------------
for p in P:
    for q in P:
        print("{0} -> {1}: {2}".format(p, q, f[p, q].X))
