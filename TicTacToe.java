import java.util.ArrayList;
import java.util.List;

public class TicTacToe {

    public static int scorePlayer(Board board, int player, int alpha, int beta)
    {
        if(board.isGameWon())
        {
            if (player ==-1){
                return -board.nFreeFields()-1;
            }
            return 1+board.nFreeFields();
        }

        if (!board.isGameWon() && board.nFreeFields()==0){
                return 0;
            }


        for (Position pos : board.validMoves()) {
            board.doMove(pos,player);
            int score = -scorePlayer(board, -player, -beta, -alpha);
            board.undoMove(pos);
            if (score > alpha) {
                alpha = score;
                if (alpha>=beta) break;
            }
        }

        //unentschieden


        return alpha;

    }



    public static int alphaBeta(Board board, int player)
    {
        // TODO
        return scorePlayer(board, player, -Integer.MAX_VALUE, Integer.MAX_VALUE);

    }


    public static void evaluatePossibleMoves(Board board, int player)
    {
        // TODO
        
    }

    public static void main(String[] args)
    {
    }
}

