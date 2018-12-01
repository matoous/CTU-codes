#!/bin/python3
# -*- encoding: utf-8 -*-

from collections import Counter


class BallsError(RuntimeError):
    """Special exception class for reporting Balls errors."""
    pass


class Balls:
    """'Group' of balls one of which is heavier/lighter.""" 

    def __init__(self, n_balls, different):
        self.n_balls = n_balls
        self.different_ball = different
        self.n_calls = 0
        self.log = ['Init: different ball set to {:n}.'.format(different[0])]

    def weigh(self, left, right):
        """Return the side which is heavier ('left', 'right', or 'none')."""
        self.n_calls += 1
        self._check_balls(left, right)
        result = self._weigh(left, right)
        self.log.append(
            'Call {}: {} vs {} --> heavier is {}'.format(self.n_calls, left, right, result)
        )
        return result
        
    def _check_balls(self, left, right):
        self._check_iterable(left)
        self._check_iterable(right)
        ball_counts = Counter()
        ball_counts.update(left)
        ball_counts.update(right)     
        if min(ball_counts.keys()) < 1:
            raise BallsError('You cannot weigh a ball with number lower than 1.')
        if max(ball_counts.keys()) > self.n_balls:
            raise BallsError('You cannot weigh a ball with number '
                             'higher than {}.'.format(self.n_balls)) 
        ball, count = ball_counts.most_common(1)[0]
        if count > 1:
            raise BallsError('You cannot use ball with number {} more than once '
                             'in a single weighing.'.format(ball))
                             
    def _check_iterable(self, a):
        try:
            _ = len(a)
        except:
            raise BallsError('Arguments "left" and "right" given to '
                             '"weigh(left, right)" '
                             'must be lists of numbers.' )  
        
    def _weigh(self, left, right):
        # Check the number of balls on each side
        if len(left) > len(right):
            return 'left'
        elif len(left) < len(right):
            return 'right'

        # Check whether any side contains the different ball
        i_different, anomaly = self.different_ball
        if i_different in left:
            if anomaly == '+':
                return 'left'
            elif anomaly == '-':
                return 'right'
        elif i_different in right:
            if anomaly == '+':
                return 'right'
            elif anomaly == '-':
                return 'left'
        return 'none'


if __name__=="__main__":
    balls = Balls(12, (7,'+'))
    balls.weigh([1], [2])
    balls.weigh([1,2], [5,6])
    balls.weigh([7,8,9], [10,11,12])
    print('The sequence of ball events was:')
    print('\n'.join(balls.log))
    print('Function weigh() was called {} times.'.format(balls.n_calls))
