class MyPlayerMobilityOneDepth:
    """ Plays best move for mobility and frontiers ratio for next round """
    # SOME STUFF WE WILL NEED

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
        if len(available_moves) == 0:
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

    def get_mobility(self, board, my_color, oponent_color):
        """

        :param my_color: my color
        :param oponent_color: oponent color
        :param board: board to get available moves
        :return: available moves
        :rtype: array of available moves
        """
        available_moves = 0
        frontiers = 0
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
                                    available_moves += 1
                                    added = True
                                    break
                                xtemp += x_change
                                ytemp += y_change
                        if added:
                            break
                if board[x][y] == my_color:
                    for x_change, y_change in self.surroundings:
                        frontier = False
                        xtemp = x + x_change
                        ytemp = y + y_change
                        if self.is_on_board(xtemp, ytemp) and board[xtemp][ytemp] == -1:
                            frontier = True
                            break
                        if frontier:
                            frontiers += 1
                            break
        return [available_moves, frontiers]

    def evaluate(self, board):
        mobility = self.get_mobility(board, self.my_color, self.opponent_color)
        return mobility[0]

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
