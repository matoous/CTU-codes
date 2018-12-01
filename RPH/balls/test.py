#!/bin/python3
# -*- encoding: utf-8 -*-

from balls import Balls
from balls_solver import solve
                  
different = (11, '-')
balls = Balls(12, different)
result = solve(balls.weigh)
print('\n'.join(balls.log))
print('After these events, my solve() function thinks '
      'that the different ball is {}.'.format(result))
print('It required {} weighings.'.format(balls.n_calls))
