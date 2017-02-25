#!/bin/python3
# -*- encoding: utf-8 -*-


def solve(weigh):
    heavier = weigh([1,2,3,4], [5,6,7,8])
    if heavier == 'left':  #left(1,2,3,4) contains heavy or right(5,6,7,8) contains light
        heavier = weigh([1,5,6],[2,7,12])
        if heavier == 'left':
            heavier = weigh([7],[10])
            if heavier == 'right':
                return 7, '-'
            else:
                return 1, '+'
        elif heavier == 'right':
            heavier = weigh([5],[6])
            if heavier == 'left':
                return 6,'-'
            elif heavier == 'right':
                return 5,'-'
            else:
                return 2,'+'
        else:
            heavier = weigh([3],[4])
            if heavier == "left":
                return 3,'+'
            elif heavier == "right":
                return 4,'+'
            else:
                return 8,'-'

    elif heavier == 'right':  #right (5,6,7,8) contains heavy or left(1,2,3,4) contains light
        heavier = weigh([1,2,5],[3,6,12])
        if heavier == "right":
            heavier = weigh([1],[2])
            if heavier == "left":
                return 2,'-'
            elif heavier == "right":
                return 1,'-'
            else:
                return 6,'+'
        elif heavier == "left":
            heavier = weigh([3],[12])
            if heavier == "right":
                return 3,'-'
            else:
                return 5,'+'
        else:
            heavier = weigh([7],[8])
            if heavier == "left":
                return 7,'+'
            elif heavier == "right":
                return 8,'+'
            else:
                return 4,'-'
    else:  # rest(9,10,11,12) contains lighter or heavier ball
        heavier = weigh([6,7,8],[9,10,11])
        if heavier == "left":
            heavier = weigh([9],[10])
            if heavier == "left":
                return 10,'-'
            elif heavier == "right":
                return 9,'-'
            else:
                return 11, '-'
        elif heavier == "right":
            heavier = weigh([9],[10])
            if heavier == "left":
                return 9,'+'
            elif heavier == "right":
                return 10,'+'
            else:
                return 11,'+'
        else:
            heavier = weigh([1],[12])
            if heavier == "right":
                return 12,'+'
            elif heavier == "left":
                return 12,'-'

