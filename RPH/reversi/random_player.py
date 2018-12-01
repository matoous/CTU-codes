from random import randint

class MyPlayerRandom:
    """ Plays random moves """

    def __init__(self, my_color, opponent_color):
        self.name = 'dzivjmat'  # username
        self.my_color = my_color  # my color
        self.opponent_color = opponent_color  # others color

    def move(self, board):
        moves = self.get_available_moves(board, self.my_color, self.opponent_color)
        if len(moves) == 0:
            return [-1,-1]
        r = randint(0, len(moves)-1)
        return moves[r]

    def get_available_moves(self, board, my_color, oponent_color):
        """

        :param my_color: my color
        :param oponent_color: oponent color
        :param board: board to get available moves
        :return: available moves
        :rtype: array of available moves
        """
        available_moves = []
        for x in range(8):
            for y in range(8):
                added = False
                if board[x][y] == -1:
                    for x_change, y_change in [[-1, -1], [-1, 0], [0, -1], [-1, 1], [1, -1], [1, 0], [0, 1], [1, 1]]:
                        xtemp = x + x_change
                        ytemp = y + y_change
                        if self.is_on_board(xtemp, ytemp) and board[xtemp][ytemp] == oponent_color:
                            while self.is_on_board(xtemp, ytemp) and board[xtemp][ytemp] != -1:
                                if board[xtemp][ytemp] == my_color:
                                    available_moves.append([x, y])
                                    added = True
                                    break
                                xtemp += x_change
                                ytemp += y_change
                        if added:
                            break
        return available_moves

    def is_on_board(self, x, y) -> bool:
        if 8 > x >= 0 and 8 > y >= 0:
            return True
        else:
            return False


if __name__ == "__player__":
    print(MyPlayer.__doc__)
