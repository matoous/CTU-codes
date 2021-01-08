#!/usr/bin/env python3
import gurobipy as g  # import Gurobi modul

# input data -------------------------------------
# SAT formula: (x0 or ~x1) and (~x0 or x1 or x2)
n_vars = 3

# model ------------------------------------------
model = g.Model()

# - add variables
x = model.addVars(n_vars, vtype=g.GRB.BINARY, name="x")

# - add constraints
#   for each clause there is one constraint
model.addConstr(x[0] + (1-x[1]) >= 1)
model.addConstr((1-x[0]) + x[1] + x[2] >= 1)

# call the solver --------------------------------
model.optimize()

# print the solution
if model.Status == g.GRB.OPTIMAL:
    print('Solution:')
    for idx in range(n_vars):
        print("x{} :".format(idx), "False" if x[idx].X < 0.5 else "True")	
else:
    print('Optimal solution was not found.')
