from time import time

class MyPlayer:
    """<img src='http://tinyurl.com/jjy5a6b' alt='Alpha beta pruning'>"""

    #  Scores for individual fields
    score_board = [[99, -12, 8, 6, 6, 8, -12, 99], [-12, -24, -4, -3, -3, -4, -24, -12], [8, -4, 7, 4, 4, 7, -4, 8],
                   [6, -3, 4, 0, 0, 4, -3, 6], [6, -3, 4, 0, 0, 4, -3, 6], [8, -4, 7, 4, 4, 7, -4, 8],
                   [-12, -24, -4, -3, -3, -4, -24, -12], [99, -12, 8, 6, 6, 8, -12, 99]]
    #  Surroundings coordinates list
    surroundings = [[-1, -1], [-1, 0], [0, -1], [-1, 1], [1, -1], [1, 0], [0, 1], [1, 1]]

    def __init__(self, my_color, opponent_color, board_size=8):
        """
        Creates instance of MyPlayer (no s*it Sherlock)
        :rtype: instance of my player
        """
        self.name = 'dzivjmat'  # username
        self.my_color = my_color  # my color
        self.opponent_color = opponent_color  # opponents color
        self.max_depth = 4  # max search tree depth
        self.board_size = board_size
        self.rounds = 0
        self.game_time = 1  # will help with setting tatctics, described later
        self.corners = ((0, 0), (self.board_size - 1, 0), (0, self.board_size - 1), (self.board_size - 1, self.board_size - 1))
        if my_color == 0:
            self.played_rounds = 0
        else:
            self.played_rounds = 1

    def move(self, board):
        """
        That is where the magic happens and the good turns arise from deepest programming hell
        Just kidding, sets alpha and beta to virtual infinities and starts alpha beta pruning search (first node is maximazing node)
        :param board: board
        :return: best move for given position
        """
        self.adjust()  # set strategy
        self.start_time = time()  # start time
        alpha, beta = -100000.0, 100000.0
        possible_moves = self.get_available_moves(board, self.my_color, self.opponent_color)
        for [x, y] in possible_moves:
            iter_start = time()  # this iteration start time
            new_board = self.make_board_copy(board)
            new_board = self.flip(new_board, x, y, self.my_color, self.opponent_color)
            new_alpha = self.minmax(new_board, 1, alpha, beta)
            if new_alpha > alpha:  # choosing best subtree/move
                alpha = new_alpha
                best_move = (x, y)
            iter_length = time() - iter_start
            if time() + iter_length >= self.start_time + 1:  # stop if next iteration would break the time limit
                break
        for corner in self.corners:  # if placing corner stone, change the static score board, so that it is good to play around this corner stone
            if corner == best_move:
                for a,b in self.surroundings:
                    if self.is_on_board(x+a, y+b):
                        self.score_board[x+a][y+b] += 24
        return best_move

    def adjust(self):
        """
        Adjusts the player during the game. Increases rounds count and changes game_time (needed for scoring)
        and the search deepnes accordingly
        """
        self.rounds += 2
        if self.rounds >= 12:
            self.max_depth= 3
        if self.rounds >= 16:
            self.game_time = 2  # early mid game
        if self.rounds >= 36:
            self.game_time = 3  # late mid game
        if self.rounds >= 46:
            self.max_depth = 4
        if self.rounds >= 50:
            self.max_depth = 5
        if self.rounds >= 54:
            self.game_time = 4  # end game
            self.max_depth = 7

    def minmax(self, board, depth, alpha, beta):
        """
        Min-max algorithm (with alpha beta pruning) searching for best options in given subtree and returning score of this subtree
        :param board: board we are currently working with
        :param depth: depth we are at
        :param alpha: alpha
        :param beta: beta
        :return: score of this node / subtree
        """
        if depth == self.max_depth:  # end point of search tree - leaf
            if time() >= self.start_time + 0.95:  # stop if time limit exceeted and return really bad score, just to be sure
                return -10000
            return self.evaluate(board)
        else:
            if depth % 2 == 0:  # my - maximizing move
                possible_moves = self.get_available_moves(board, self.my_color, self.opponent_color)
                if len(possible_moves) == 0:
                    return self.minmax(board, depth + 1, alpha, beta)
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
                    return self.minmax(board, depth + 1, alpha, beta)
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
        Gets board numbers for all different things
        :param board: board to evaluate
        :return: score of the board relative to this player
        """
        my_score = 0
        opponent_score = 0
        my_material = 0
        opponent_material = 0
        my_frontiers = 0
        opponent_frontiers = 0
        my_mobility = len(self.get_available_moves(board, self.my_color, self.opponent_color))
        opponent_mobility = len(self.get_available_moves(board, self.opponent_color, self.my_color))
        for x in range(self.board_size):
            for y in range(self.board_size):
                if board[x][y] == self.my_color:  # my field
                    my_score += self.score_board[x][y]
                    my_material += 1
                    for x_ch, y_ch in self.surroundings:
                        if self.is_on_board(x + x_ch, y + y_ch) and board[x + x_ch][y + y_ch] == -1:
                            break
                    else:
                        my_frontiers += 1 # cool python trick, else after for loop gets executed if you break out of the for loop
                elif board[x][y] == self.opponent_color:  # opponent field
                    opponent_score += self.score_board[x][y]
                    opponent_material += 1
                    for x_ch, y_ch in self.surroundings:
                        if self.is_on_board(x + x_ch, y + y_ch) and board[x + x_ch][y + y_ch] == -1:
                            break
                    else:
                        opponent_frontiers += 1
        return self.score(my_score, my_material, my_mobility, my_frontiers, opponent_score, opponent_material, opponent_mobility, opponent_frontiers)

    def score(self, my_score, my_material, my_mobility, my_frontiers, opponent_score, opponent_material, opponent_mobility, opponent_frontiers):
        """
        Scores the board base on strategies for different parts of game
        1: early game
        2: early mid game
        3: mid game
        4: endgame
        :param my_score:
        :param my_material:
        :param my_mobility:
        :param opponent_material:
        :param opponent_mobility:
        :return:
        """
        # zero division errors prevention
        if opponent_score == 0:
            opponent_score = 1
        if my_material == 0:
            my_material = 1
        if opponent_mobility == 0:
            opponent_mobility = 1
        if my_frontiers == 0:
            my_frontiers = 1

        # Also this is one big bullshit
        # It would be way better to do some hyperparameter tunning than to randomly guess the numbers

        if self.game_time == 1:
            # get mobility, still count with score so we dont jump in bad position and try do decrease number of stones
            return (my_mobility/opponent_mobility) * (my_score - opponent_score) * (opponent_material/my_material)
        if self.game_time == 2:
            # mobility is very important now, try to reduce frontiers and still count with score
            return pow((my_mobility/opponent_mobility), 2) * (opponent_frontiers / my_frontiers) * (my_score - opponent_score)
        if self.game_time == 3:
            # score is important to take good corner positions, still try to maintain mobility
            return  (my_score - opponent_score) * (my_mobility / opponent_mobility)
        if self.game_time == 4:
            # now I am evaluating to the end of the game, try to get as many stones as possible
            return my_material - opponent_material
        return (my_score - opponent_frontiers) * (my_mobility / opponent_mobility)  # this really should not happen, but who knows

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
        sorted_moves = []
        for x in range(self.board_size):
            for y in range(self.board_size):
                added = False
                if board[x][y] == -1:  # if it is an empty space
                    for x_change, y_change in self.surroundings:
                        xtemp = x + x_change
                        ytemp = y + y_change
                        if self.is_on_board(xtemp, ytemp) and board[xtemp][ytemp] == opponent_color:  # if opponent has a square next to this one
                            while self.is_on_board(xtemp, ytemp) and board[xtemp][ytemp] != -1:
                                if board[xtemp][ytemp] == my_color:
                                    available_moves.append([x, y])
                                    added = True
                                    break
                                xtemp += x_change
                                ytemp += y_change
                        if added:
                            break
        # make the corners first, so we maximalize utility of alpha beta pruning (corners will often generate good score and cut a lot)
        for move in available_moves:
            if tuple(move) in self.corners:
                sorted_moves.append([x,y])
                available_moves.remove([x,y])
        for a in available_moves:
            sorted_moves.append(a)
        return sorted_moves

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
                            the_board[xtemp][ytemp] = placed_color  # flip the stones along the way back
                            xtemp -= x_change
                            ytemp -= y_change
                        break
                    xtemp += x_change
                    ytemp += y_change
        return the_board

    def is_on_board(self, x, y):
        """
        Checks if coordinates still are on the board
        :param x: x coordinate
        :param y: y coordinate
        :return: boolean
        """
        return True if (self.board_size > x >= 0 and self.board_size > y >= 0) else False

    def make_board_copy(self, board):
        """
        Creates copy of board
        :param board: board, which should be copied
        :return: new board
        """
        new_board = [[board[b][a] for a in range(self.board_size)] for b in range(self.board_size)]
        return new_board


if __name__ == "__main__":
    print(MyPlayer.__doc__)
    player = MyPlayer(0, 1)
