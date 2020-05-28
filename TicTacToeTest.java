import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.fail;

class TicTacToeTest {

	@Test
	void testAlphaBetaBoardInt() {

	Board testBoard = new Board(3);

	testBoard.BrettSpiel[0][0]=1;
	testBoard.BrettSpiel[0][0]=-1;
	testBoard.BrettSpiel[0][0]=1;

	testBoard.BrettSpiel[1][0]=0;
	testBoard.BrettSpiel[1][1]=1;
	testBoard.BrettSpiel[1][2]=0;

	testBoard.BrettSpiel[2][0]=-1;
	testBoard.BrettSpiel[2][1]=0;
	testBoard.BrettSpiel[2][2]=-1;

	TicTacToe testTicTacToe = new TicTacToe();


	int score = testTicTacToe.alphaBeta(testBoard,1);

	System.out.println(score);
		//fail("Not yet implemented");
	}

	@Test
	void testEvaluatePossibleMoves() {
		fail("Not yet implemented");
	}
}


