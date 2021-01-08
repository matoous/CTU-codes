#!/usr/bin/env python3
import gurobipy as g  # import Gurobi modul

# data -------------------------------------------------------
n = 5  # number of vertices
edges = [(0, 1), (2, 3), (0, 4), (3, 1), (3, 4)]  # list of edges

# model ------------------------------------------------------
model = g.Model()

# - add variables
x = model.addVars(n, vtype=g.GRB.BINARY, name="x")

# - add constraints
for (i, j) in edges:
    model.addConstr(x[i] + x[j] <= 1)
	
# - set objective
model.setObjective(x.sum(), g.GRB.MAXIMIZE)

# call the solver --------------------------------------------
model.optimize()

# print the solution -----------------------------------------
for idx in range(n):
    print(idx, x[idx].X)
