#!/usr/bin/env python3
import gurobipy as g  # import Gurobi module

# data ----------------------------
#  minimize -x + 2y s.t.
#     -4x - 9y <= -18
#  (3/2)x -  y <= 27/4
# (8/17)x -  y >= -2 

# model ---------------------------
model = g.Model()

# - add variables
x = model.addVar(vtype=g.GRB.CONTINUOUS, name="x")
y = model.addVar(vtype=g.GRB.CONTINUOUS, name="y")

# - add constraints
model.addConstr(-4*x - 9*y <= -18)
model.addConstr(3/2*x - y <= 27/4)
model.addConstr(8/17*x - y >= -2)

# - set objective
model.setObjective(-x + 2*y, g.GRB.MINIMIZE)

# call the solver -----------------
model.optimize()

# print the solution --------------
print("Objective:", model.objVal)
print("x:", x.X)
print("y:", y.X)
