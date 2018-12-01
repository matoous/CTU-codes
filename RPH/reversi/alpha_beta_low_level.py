from random import randint


class MyPlayerLow:
    """ Min max low search """

    #  Scores for individual fields
    score_board = [[99, -12, 8, 6, 6, 8, -12, 99], [-12, -24, -4, -3, -3, -4, -24, -12], [8, -4, 7, 4, 4, 7, -4, 8],
                   [6, -3, 4, 0, 0, 4, -3, 6], [6, -3, 4, 0, 0, 4, -3, 6], [8, -4, 7, 4, 4, 7, -4, 8],
                   [-12, -24, -4, -3, -3, -4, -24, -12], [99, -12, 8, 6, 6, 8, -12, 99]]
    #  Helping array
    surroundings = [[-1, -1], [-1, 0], [0, -1], [-1, 1], [1, -1], [1, 0], [0, 1], [1, 1]]
    #  Max depth of recursive move searching
    max_depth = 3

    def __init__(self, my_color, opponent_color):
        """
        Creates instance of MyPlayer with my nickname
        COOL
        :rtype: instance of my player
        """
        self.name = 'dzivjmat'  # username
        self.my_color = my_color  # my color
        self.opponent_color = opponent_color  # others color
        if my_color == 0:
            self.played_rounds = 0

    def move(self, board):
        """
        That is where the magic happens and the good turns arise from deepest programming hell
        :param board: board
        :return: best move for given position
        """
        best_move = []
        alpha = -100000  # virtual -infinity
        beta = 100000  # virtual +infinity
        possible_moves = self.get_available_moves(board, self.my_color, self.opponent_color)
        if len(possible_moves) == 0:  # for testing purposes
            return [-1, -1]
        for [x, y] in possible_moves:
            new_board = self.make_board_copy(board)
            new_board = self.flip(new_board, x, y, self.my_color, self.opponent_color)
            new_alpha = self.minmax(new_board, 1, alpha, beta)
            if new_alpha > alpha:  # choosing best subtree
                alpha = new_alpha
                best_move = [x, y]
        if len(best_move) == 0:  # for testing purposes
            print(possible_moves)
            best_move = possible_moves[randint(0, len(possible_moves) - 1)]
        return best_move

    def minmax(self, board, depth, alpha, beta):
        """
        Min-max algorithm searching for best options in given subtree and returning score of this subtree
        :param board: board we are currently working with
        :param depth: depth we are at
        :param alpha: alpha
        :param beta: beta
        :return: score of this node / subtree
        """
        if depth == self.max_depth:  # end point of search tree
            return self.evaluate(board)
        else:
            if depth % 2 == 0:  # my - maximizing move
                possible_moves = self.get_available_moves(board, self.my_color, self.opponent_color)
                if len(possible_moves) == 0:
                    return self.evaluate(board)
                for [x, y] in possible_moves:
                    new_board = self.make_board_copy(board)
                    new_board = self.flip(new_board, x, y, self.my_color, self.opponent_color)
                    new_alpha = self.minmax(new_board, depth + 1, alpha, beta)
                    if new_alpha > alpha:  # update best possible score from subtree
                        alpha = new_alpha
                    if alpha >= beta:
                        return alpha
                return alpha
            else:  # opponent - minimizing move
                possible_moves = self.get_available_moves(board, self.opponent_color, self.my_color)
                if len(possible_moves) == 0:
                    return self.evaluate(board)
                for [x, y] in possible_moves:
                    new_board = self.make_board_copy(board)
                    new_board = self.flip(new_board, x, y, self.opponent_color, self.my_color)
                    new_beta = self.minmax(new_board, depth + 1, alpha, beta)
                    if new_beta < beta:  # update worst possible score from subtree
                        beta = new_beta
                    if beta <= alpha:
                        return beta
                return beta

    def evaluate(self, board):
        """
        Scores the board base on many things
        :param board: board to evaluate
        :return: score of the board relative to this player
        """
        my_score = 0
        my_material = 0
        opponent_material = 0
        my_mobility = len(self.get_available_moves(board, self.my_color, self.opponent_color))
        opponent_mobility = len(
            self.get_available_moves(board, self.opponent_color, self.my_color))  # THIS ALL CAN BE ONE METHOD
        for x in range(8):
            for y in range(8):
                if board[x][y] == self.my_color:
                    my_score += self.score_board[x][y]
                    my_material += 1
                elif board[x][y] == self.opponent_color:
                    opponent_material += 1
        if opponent_mobility == 0 or my_mobility == 0:  # ONLY FOR MY GAME
            return my_score  # ---------------------------DELETE LATER
        return self.score(my_score, my_material, my_mobility, opponent_material, opponent_mobility)

    def score(self, my_score, my_material, my_mobility, opponent_material, opponent_mobility):
        """
        Scores the board base on strategies for different parts of game
        :param my_score:
        :param my_material:
        :param my_mobility:
        :param opponent_material:
        :param opponent_mobility:
        :return:
        """
        return my_score * pow((opponent_material / my_material), 2) * (my_mobility / opponent_mobility)

    def get_available_moves(self, board, my_color, opponent_color):
        """
        Gets list of available moves, sorted from top left corner to bottom right, line by line
        :param my_color: my color
        :param opponent_color: opponent color
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
                        if self.is_on_board(xtemp, ytemp) and board[xtemp][ytemp] == opponent_color:
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

    def flip(self, the_board, x, y, placed_color, opponent_color):
        """
        Flips stones that should be flipped and returns back the board
        :param the_board: game board
        :param x: x coordinate of added stone
        :param y: y coordinate of added stone
        :param placed_color: color of added stone
        :param opponent_color: color of opponents stones
        :return: board with current game state
        """
        the_board[x][y] = placed_color
        for x_change, y_change in self.surroundings:
            xtemp = x + x_change
            ytemp = y + y_change
            if self.is_on_board(xtemp, ytemp) and the_board[xtemp][ytemp] == opponent_color:
                while self.is_on_board(xtemp, ytemp) and the_board[xtemp][ytemp] != -1:
                    if the_board[xtemp][ytemp] == placed_color:
                        xtemp -= x_change
                        ytemp -= y_change
                        while self.is_on_board(xtemp, ytemp) and the_board[xtemp][ytemp] != placed_color:
                            the_board[xtemp][ytemp] = placed_color
                            xtemp -= x_change
                            ytemp -= y_change
                        break
                    xtemp += x_change
                    ytemp += y_change
        return the_board

    def is_on_board(self, x, y) -> bool:
        """
        Checks if coordinates still are on the board
        :param x: x coordinate
        :param y: y coordinate
        :return: boolean
        """
        if 8 > x >= 0 and 8 > y >= 0:
            return True
        else:
            return False

    def make_board_copy(self, board):
        """
        Creates copy of board
        :param board: board, which should be copied
        :return: new board
        """
        new_board = [[board[b][a] for a in range(8)] for b in range(8)]
        return new_board


# ----------------------------------------------------------------------------------------------------------------------
# ___IF CALLED AS MAIN_____
if __name__ == "__main__":
    print(MyPlayer.__doc__)
    player = MyPlayer(0, 1)
