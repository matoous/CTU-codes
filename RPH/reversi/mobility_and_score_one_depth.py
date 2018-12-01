class MyPlayerMobilityAndScore:
    """ Plays best move for next round base on field score and mobility/frontiers ratio """
    # SOME STUFF WE WILL NEED
    score_board = [[99, -8, 8, 6, 6, 8, -8, 99], [-8, -24, -4, -3, -3, -4, -24, -8], [8, -4, 7, 4, 4, 7, -4, 8],
                   [6, -3, 4, 0, 0, 4, -3, 6], [6, -3, 4, 0, 0, 4, -3, 6], [8, -4, 7, 4, 4, 7, -4, 8],
                   [-8, -24, -4, -3, -3, -4, -24, -8], [99, -8, 8, 6, 6, 8, -8, 99]]

    surroundings = [[-1, -1], [-1, 0], [0, -1], [-1, 1], [1, -1], [1, 0], [0, 1], [1, 1]]

    # INIT - predefined
    def __init__(self, my_color, opponent_color):
        """

        :rtype: instance of my player
        """
        self.name = 'dzivjmat'  # username
        self.my_color = my_color  # my color
        self.opponent_color = opponent_color  # others color

    # MAKE MOVE - predefined
    def move(self, board):
        move = self.get_best_move(board)
        return move

    # MY STUFF HERE
    def get_best_move(self, board):
        available_moves = self.get_available_moves(board, self.my_color, self.opponent_color)
        best_move = []
        score_of_best = -10000
        for [x, y] in available_moves:
            new_board = self.make_board_copy(board)
            new_board[x][y] = self.my_color
            score = self.evaluate(new_board)
            if score > score_of_best:
                best_move = [x, y]
                score_of_best = score
        if len(best_move) == 0:
            return [-1,-1]
        return best_move

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
                    for x_change, y_change in self.surroundings:
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

    def evaluate(self, board):
        score = self.get_board_score(board)
        return score

    def get_board_score(self, board):
        my_score = 0
        my_mobility = self.get_available_moves(board,self.my_color,self.opponent_color)
        opponent_mobility = self.get_available_moves(board,self.opponent_color,self.my_color)
        for x in range(8):
            for y in range(8):
                if board[x][y] == self.my_color:
                    my_score += self.score_board[x][y]
        if len(opponent_mobility) == 0 or len(my_mobility) == 0:
            return my_score
        return my_score * (pow((len(my_mobility) / len(opponent_mobility)),2))

    # -------------------------
    # -------------------------
    # -------------------------
    # -------------------------
    # ________COMPLETE_________
    def is_on_board(self, x, y) -> bool:
        """

        :param x: x coordinate
        :param y: y coordinate
        :return: boolean
        """
        if 8 > x >= 0 and 8 > y >= 0:
            return True
        else:
            return False

    def make_board_copy(self, board):
        new_board = [[0 for a in range(8)] for b in range(8)]
        for x in range(8):
            for y in range(8):
                new_board[x][y] = board[x][y]
        return new_board

# -------------------------
# -------------------------
# -------------------------
# -------------------------
# ___IF CALLED AS MAIN_____
if __name__ == "__main__":
    print(MyPlayer.__doc__)
