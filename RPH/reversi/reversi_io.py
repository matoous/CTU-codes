import csv

class ReversiIO(object):
    
    @staticmethod
    def save_state_with_moves(file_name,board,moves,move_color):
        with open(file_name, 'wb') as outfile:
            for x in range(len(board)):
                row_string = ''
                for y in range(len(board[x])):
                    row_string += str(board[x][y])+' '
                outfile.write(row_string+'\n')
            outfile.write('\n')
            outfile.write(str(move_color)+'\n')
            outfile.write('\n')
            for i in range(len(moves)):
                row_string = str(moves[i][0])+' '+str(moves[i][1])+' '
                outfile.write(row_string+'\n')
            
            
    @staticmethod
    def load_state_with_moves(file_name):
        board_size = 8 
        board = [-1]*board_size
        player = -1;
        moves = []
        for row in range(board_size):
            board[row] = [-1]*board_size
        with open(file_name, 'r') as f:
            lines = f.readlines()
            for linei in range(0,8):
                stones = lines[linei].split()
                for st_id in range(len(stones)):
                    board[linei][st_id] = int(stones[st_id])
            player = int(lines[9])
            for linei in range(11,len(lines)):
                move = lines[linei].split()
                moves.append([int(move[0]) , int(move[1])])
            return {'board':board,'player':player,'moves':moves}        
        