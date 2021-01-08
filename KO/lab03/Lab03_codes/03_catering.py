# Problem description: Catering company has one oven to cook n dishes. Each dish has its 
# earliest time to be put to the oven r, the latest time it should be taken from the 
# oven d, and the time it needs to be cooked in the oven p. Find such order of dishes that 
# the finish time of the oven is minimal.

import gurobipy as g  # import Gurobi module

# data ------------------------------------------------------
n = 5
orders = {
    0: {'r': 20, 'd': 45, 'p': 15},
    1: {'r': 4, 'd': 30, 'p': 19},
    2: {'r': 5, 'd': 80, 'p': 20},
    3: {'r': 17, 'd': 70, 'p': 8},
    4: {'r': 27, 'd': 66, 'p': 7}
}

# model -----------------------------------------------------
m = g.Model()

# - add variables
s = m.addVars(n, vtype=g.GRB.CONTINUOUS, lb=0, name="s")
x = {}
for i in range(n):
    for j in range(i):
        x[i, j] = m.addVar(vtype=g.GRB.BINARY, name="x")

Cmax = m.addVar(vtype=g.GRB.CONTINUOUS, name="Cmax")

# - add constraints
for i in range(n):
    m.addConstr(s[i] + orders[i]['p'] <= Cmax)
    m.addConstr(s[i] >= orders[i]['r'])
    m.addConstr(s[i] + orders[i]['p'] <= orders[i]['d'])

M = sum([orders[i]['p']+orders[i]['r'] for i in range(n)])
#M = max([orders[i]['d'] for i in range(n)])

for i in range(n):
    for j in range(i):
        m.addConstr(s[i] + orders[i]['p'] <= s[j] + M*(1-x[i, j]))
        m.addConstr(s[j] + orders[j]['p'] <= s[i] + M*x[i, j])

# - set objective
m.setObjective(Cmax, g.GRB.MINIMIZE)

# call the solver -------------------------------------------
m.optimize()

# print the solution ----------------------------------------
print('solution:')
for i in range(n):
    print('order {} starts baking at: {}'.format(i, int(round(s[i].x))))