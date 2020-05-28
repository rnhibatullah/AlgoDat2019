import java.util.*;

import static java.lang.Math.abs;

public class Board {
    private int n;
    public int BrettSpiel[][];
    public int whoWins=0;

    public boolean win(int player){



        //search for horizontal
        int resultHorizontal=0;
        for (int i=0; i<n; i++){
            int sumHorizontal =0;
            for (int j=0;j<n;j++){
                sumHorizontal = sumHorizontal + BrettSpiel[i][j];
            }
            resultHorizontal = n*player;
            if(sumHorizontal == resultHorizontal){
                return true;
            }
        }

        //search for vertical
        int resultVertical;
        for (int j=0; j<n; j++){
            int sumVertical =0;
            for (int i=0;i<n;i++){
                sumVertical = sumVertical + BrettSpiel[i][j];
            }
            resultVertical = n*player;
            if(sumVertical == resultVertical){
                return true;
            }
        }

        //search first diagonal
        int sumDigonal1 =0;
        for (int i=0;i<n; i++){
            sumDigonal1 = sumDigonal1 + BrettSpiel[i][i];

        }
        if (sumDigonal1 == n*player){
            return true;
        }

        //search second Diagonal
        int sumDigonal2 =0;
        for (int i=0;i<n; i++){
            sumDigonal2 = sumDigonal2 + BrettSpiel[i][n-1-i];

        }
        if (sumDigonal2 == n*player) {
            return true;
        }
        return false;
    }

    public Board(int n)
    {
        // TODO
        this.n=n;
        if (n<1 || n>10){
            throw new InputMismatchException("n not valid");
        }

        this.BrettSpiel = new int[n][n];
    }
    
    
    public int getN() { return n; }

    public int nFreeFields() {
        // TODO
        int freefields = 0;

        for (int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                if (BrettSpiel[i][j]==0){
                    freefields = freefields+1;
                }
            }
        }

        return freefields;
    }

    public int getField(Position pos) throws InputMismatchException
    {
        // TODO
        int x= pos.x;
        int y =pos.y;

        if (x>n){
            throw new InputMismatchException("Invalid moves in x");
        }
        if(y>n){
            throw new InputMismatchException("Invalid moves in Y");
        }

        return BrettSpiel[x][y];

    }

    public void setField(Position pos, int token) throws InputMismatchException
    {
        // TODO


        int x= pos.x;
        int y =pos.y;

        if (x>n){
            throw new InputMismatchException("Invalid moves in x");
        }

        if (x<0){
            throw new InputMismatchException("Invalid moves in x");
        }

        if(y>n){
            throw new InputMismatchException("Invalid moves in Y");
        }

        if(y<0){
            throw new InputMismatchException("Invalid moves in Y");
        }

        BrettSpiel[x][y]= token;
    }

    public void doMove(Position pos, int player)
    {
        // TODO
        int x= pos.x;
        int y =pos.y;

        if (x>n){
            throw new InputMismatchException("Invalid moves in x");
        }
        if(y>n){
            throw new InputMismatchException("Invalid moves in Y");
        }

        BrettSpiel[x][y]= player;
    }

    public void undoMove(Position pos)
    {
        // TODO
        int x= pos.x;
        int y =pos.y;

        if (x>n){
            throw new InputMismatchException("Invalid moves in x");
        }
        if(y>n){
            throw new InputMismatchException("Invalid moves in Y");
        }

        BrettSpiel[x][y]= 0;
    }

    public boolean isGameWon() {
        // TODO
        boolean player1= this.win(1);
        boolean player2= this.win(-1);

        if (player1==true){
            this.whoWins=1;
            return true;
        }

        if (player2==true){
            this.whoWins=-1;
            return true;
        }

        return false;

    }

    public Iterable<Position> validMoves()
    {
        // TODO
        List Valid = new ArrayList<Position>();

        for (int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                if (BrettSpiel[i][j]==0){
                    Position leer = new Position(i,j);
                   Valid.add(leer);
                }
            }
        }

        return Valid;

    }

    public void print()
    {
        // TODO
        for (int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                System.out.println(BrettSpiel[i][j]);
                }
            }

    }

}

