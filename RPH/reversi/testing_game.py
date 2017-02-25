from player import MyPlayer
from mobility_and_score_one_depth import MyPlayerMobilityAndScore

######################################################################

comments = True

surroundings = [[-1, -1], [-1, 0], [0, -1], [-1, 1], [1, -1], [1, 0], [0, 1], [1, 1]]

######################################################################

def wait():
    input()

def print_board(board):
    '''
    Function for printing game board to console.
    '''
    for x in range(len(board)):
        row_string = ''
        for y in range(len(board[x])):
            if board[x][y] == -1:
                row_string += ' -'
            else:
                row_string += ' ' + str(board[x][y])
        print(row_string)

def board_full(board):
    for x in range(8):
        for y in range(8):
            if board[x][y] == -1:
                return False
    return True

def init_board():
    new_board = [[0 for a in range(8)] for b in range(8)]
    for x in range(8):
        for y in range(8):
            new_board[x][y] = -1
    new_board[3][3] = 1
    new_board[4][4] = 1
    new_board[3][4] = 0
    new_board[4][3] = 0
    return new_board


def is_on_board(x, y) -> bool:
    """

    :param x: x coordinate
    :param y: y coordinate
    :return: boolean
    """
    if 8 > x >= 0 and 8 > y >= 0:
        return True
    else:
        return False


def flip(board,x,y,my_color, opponent_color):
    board[x][y] = my_color
    for x_change, y_change in surroundings:
        xtemp = x + x_change
        ytemp = y + y_change
        if is_on_board(xtemp, ytemp) and board[xtemp][ytemp] == opponent_color:
            while is_on_board(xtemp, ytemp) and board[xtemp][ytemp] != -1:
                if board[xtemp][ytemp] == my_color:
                    xtemp -= x_change
                    ytemp -= y_change
                    while is_on_board(xtemp, ytemp) and board[xtemp][ytemp] != my_color:
                        board[xtemp][ytemp] = my_color
                        xtemp -= x_change
                        ytemp -= y_change
                    break
                xtemp += x_change
                ytemp += y_change
    return board

def who_won(board):
    zeroes = 0
    ones = 0
    for x in range(8):
        for y in range(8):
            if board[x][y] == 1:
                ones += 1
            elif board[x][y] == 0:
                zeroes += 1
    if zeroes > ones:
        return 0, zeroes, ones
    elif ones > zeroes:
        return 1, ones, zeroes
    else:
        return 2, ones, zeroes


if __name__ == "__main__":
    player1_wins = 0
    player2_wins = 0
    ties = 0
    for i in range(10):
        player1 = MyPlayerMobilityAndScore(0, 1)
        player2 = MyPlayer(1, 0)
        board = init_board()
        passed = 0
        if comments:
            print_board(board)
        while not board_full(board) and not passed >= 2:
            x, y = player1.move(board)
            if not(x == -1 and y == -1):
                passed = 0
                board = flip(board, x, y, 0, 1)
                if comments:
                    print("Player 1 moves to:", x, ":", y)
                    print_board(board)
            else:
                passed += 1
            x, y = player2.move(board)
            if not(x == -1 and y == -1):
                passed = 0
                board = flip(board, x, y, 1, 0)
                if comments:
                    print("Player 2 moves to:", x, ":", y)
                    print_board(board)
            else:
                passed += 1
        winner, winnerpoints, looserpoints = who_won(board)
        if winner == 0:
            print("Zvitezil hrac 1:", player1.__doc__, "s", winnerpoints,"body")
            player1_wins += 1
            print("Druhy byl hrac 2:", player2.__doc__, "s", looserpoints, "body")
        elif winner == 1:
            print("Zvitezil hrac 2:", player2.__doc__, "s", winnerpoints,"body")
            player2_wins += 1
            print("Druhy byl hrac 1:", player1.__doc__, "s", looserpoints, "body")
        elif winner == 2:
            ties += 1
    print("Hrac 1:",player1.__doc__,"vyhral",player1_wins,"x a Hrac 2:", player2.__doc__,"vyhral",player2_wins,"x")
    print(ties,"x remiza")


