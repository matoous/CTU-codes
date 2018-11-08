package cz.cvut.student;

import cz.cvut.fel.aic.zui.gobblet.Gobblet;
import cz.cvut.fel.aic.zui.gobblet.algorithm.Algorithm;
import cz.cvut.fel.aic.zui.gobblet.environment.Board;
import cz.cvut.fel.aic.zui.gobblet.environment.Move;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Collectors;

import static java.lang.Integer.min;
import static java.lang.Math.max;

class Transposition {
    public Long first;
    public Integer second;

    public Transposition(Long first, Integer second) {
        super();
        this.first = first;
        this.second = second;
    }

    public int hashCode() {
        int hashFirst = first != null ? first.hashCode() : 0;
        int hashSecond = second != null ? second.hashCode() : 0;

        return (hashFirst + hashSecond) * hashSecond + hashFirst;
    }

    public boolean equals(Object other) {
        if (other instanceof Transposition) {
            Transposition otherPair = (Transposition) other;
            return
                    ((  this.first == otherPair.first ||
                            ( this.first != null && otherPair.first != null &&
                                    this.first.equals(otherPair.first))) &&
                            (  this.second == otherPair.second ||
                                    ( this.second != null && otherPair.second != null &&
                                            this.second.equals(otherPair.second))) );
        }

        return false;
    }
}

class TranspositionValue {
    static Short EXACT = 1;
    static Short LOWER = 2;
    static Short UPPER = 3;

    public Integer value;
    public Short flag;

    public TranspositionValue(Integer value, Short flag) {
        super();
        this.value = value;
        this.flag = flag;
    }

    public int hashCode() {
        int hashFirst = value != null ? value.hashCode() : 0;
        int hashSecond = flag != null ? flag.hashCode() : 0;

        return (hashFirst + hashSecond) * hashSecond + hashFirst;
    }

    public boolean equals(Object other) {
        if (other instanceof TranspositionValue) {
            TranspositionValue otherPair = (TranspositionValue) other;
            return
                    ((  this.value == otherPair.value ||
                            ( this.value != null && otherPair.value != null &&
                                    this.value.equals(otherPair.value))) &&
                            (  this.flag == otherPair.flag ||
                                    ( this.flag != null && otherPair.flag != null &&
                                            this.flag.equals(otherPair.flag))) );
        }

        return false;
    }
}



public class AlphaBeta extends Algorithm {
    int maximazing = -100;
    private Map<Transposition, TranspositionValue> table = new HashMap<>();

    @Override
    protected int runImplementation(Board game, int depth, int player, int alpha, int beta) {
        if (this.maximazing == -100) {
            this.maximazing = player;
        }

        int alphaOrig = alpha;

        Transposition key = new Transposition(game.calculateSimpleHash(), depth);
        if (table.containsKey(key)) {
            TranspositionValue trans = table.get(key);
            if (trans.flag == TranspositionValue.EXACT) {
                return trans.value;
            } else if (trans.flag == TranspositionValue.UPPER) {
                beta = Math.min(beta, trans.value);
            } else if (trans.flag == TranspositionValue.LOWER) {
                alpha = Math.max(alpha, trans.value);
            }
            if (alpha >= beta) {
                return trans.value;
            }
        }

        if (depth == 0 || game.isTerminate(player) != Board.DUMMY) {
            return maximazing == Board.WHITE_PLAYER ? game.evaluateBoard() : -game.evaluateBoard();
        }

        ArrayList<Move> successors = game.generatePossibleMoves(player);
        ArrayList<Board> boards = successors.stream().map(succ -> {
            Board sub = new Board(game);
            sub.makeMove(succ);
            return sub;
        }).sorted((o1, o2) -> player == Board.WHITE_PLAYER ? o2.evaluateBoard() - o1.evaluateBoard() : o1.evaluateBoard() - o2.evaluateBoard()).collect(Collectors.toCollection(ArrayList::new));

        int v;
        if (player == this.maximazing) {
            v = Integer.MIN_VALUE;
            for (Board b : boards) {
                v = max(run(b, depth - 1, player == Board.WHITE_PLAYER ? Board.BLACK : Board.WHITE_PLAYER, alpha, beta), v);
                alpha = max(alpha, v);
                if (beta <= alpha) {
                    break;
                }
            }
        } else {
            v = Integer.MAX_VALUE;
            for (Board b : boards) {
                v = min(run(b, depth - 1, player == Board.WHITE_PLAYER ? Board.BLACK : Board.WHITE_PLAYER, alpha, beta), v);
                beta = min(beta, v);
                if (beta <= alpha) {
                    break;
                }
            }
        }

        Short flag = v <= alphaOrig ? TranspositionValue.UPPER : alpha >= beta ? TranspositionValue.LOWER : TranspositionValue.EXACT;
        TranspositionValue value = new TranspositionValue(alpha, flag);
        table.put(key, value);

        return v;
    }
}