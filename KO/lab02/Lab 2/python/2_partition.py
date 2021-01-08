#!/usr/bin/env python3
import gurobipy as g  # import Gurobi module

# data -----------------------------------------
p = [100, 50, 50, 50, 20, 20, 10]

# model ----------------------------------------
model = g.Model()

# - add variables
x = model.addVars(len(p), vtype=g.GRB.BINARY, name="x")

# - add constraints
model.addConstr(g.quicksum([x[idx] * p[idx] for idx in range(len(p))]) == g.quicksum([(1-x[idx]) * p[idx] for idx in range(len(p))]))

# - set objective
# we look for any feasible solution, objective is not necessary

# call the solver ------------------------------
model.optimize()

# print the solution ---------------------------
if model.Status == g.GRB.OPTIMAL:
    for idx in range(len(p)):
        print(p[idx], "in S1" if (x[idx].X > 0.5) else "in S2")
else:
    print("Optimal solution was not found.")
