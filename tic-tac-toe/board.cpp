#include <iostream>
#include <cmath>
#include "board.h"

using namespace std;

Board::Board()
{
    currentPlayer = 1;
    dimension = 3;
    board = new int*[dimension];
	for (int i=0; i<dimension; i++)
    {
        board[i] = new int[dimension];
        for (int j=0;j<dimension; j++)
		{
            board[i][j] = 0;
		}
	}
}

Board::Board(int a)
{
    currentPlayer = 1;
	dimension = a;
    board = new int*[dimension];
	for (int i=0; i<dimension; i++)
    {
		board[i] = new int[dimension];
	    for (int j=0; j<dimension; j++)
        {
			board[i][j] = 0;
		}
	}
}

Board::Board(Board& b1)
{
    this->currentPlayer = b1.currentPlayer;
    this->dimension = b1.dimension;
    board = new int*[dimension];
    for (int i=0; i<dimension; i++)
    {
        board[i] = new int[dimension];
        for (int j=0; j<dimension; j++)
        {
            board[i][j] = b1.board[i][j];
        }
    }
}

void Board::makeMove(int row, int col) // update the board
{
    if (board[row][col]!=0)
    {
        cout << "Cell already occupied. Invalid move. You lose your turn." << endl;
    }
    else if (board[row][col]==0)
    {
        board[row][col] = currentPlayer;
    }
    if (currentPlayer == 1)
    {
        currentPlayer = 2;
    }
    else if (currentPlayer == 2)
    {
        currentPlayer = 1;
	}
}

int Board::checkWin() // checks if someone has won the game
{
	//Last condition to ensure checkWin is not true for empty board
    if (board[0][0] == board[0][1] && board[0][0] == board [0][2] && board[0][0]!=0)
    {
        return true;
    }
    else if (board[1][0] == board[1][1] && board[1][0] == board [1][2] && board[1][0]!=0)
    {
        return true;
    }
    else if (board[2][0] == board[2][1] && board[2][0] == board [2][2] && board[2][0]!=0)
    {
        return true;
    }
    else if (board[0][0] == board[1][0] && board[0][0] == board [2][0] && board[0][0]!=0)
    {
        return true;
    }
    else if (board[0][1] == board[1][1] && board[0][1] == board [2][1] && board[0][1]!=0)
    {
        return true;
    }
    else if (board[0][2] == board[1][2] && board[0][2] == board [2][2] && board[0][2]!=0)
    {
        return true;
    }
    else if (board[0][0] == board[1][1] && board[0][0] == board [2][2] && board[0][0]!=0)
    {
        return true;
    }
    else if (board[0][2] == board[1][1] && board[0][2] == board [2][0]  && board[0][2]!=0)
    {
        return true;
    }
    return false;
}

int Board::checkP1Win()
{
    if (board[0][0] == 1 && board[0][1] == 1 && board [0][2] ==1)
    {
        return true;
    }
    else if (board[1][0] == 1 && board[1][1] == 1 && board [1][2] == 1)
    {
        return true;
    }
    else if (board[2][0] == 1 && board[2][1] == 1 && board [2][2] == 1)
    {
        return true;
    }
    else if (board[0][0] == 1 && board[1][0] == 1 && board [2][0] == 1)
    {
        return true;
    }
    else if (board[0][1] == 1 && board[1][1] == 1 && board [2][1] == 1)
    {
        return true;
    }
    else if (board[0][2] == 1 && board[1][2] == 1 && board [2][2] == 1)
    {
        return true;
    }
    else if (board[0][0] == 1 && board[1][1] == 1 && board [2][2] == 1)
    {
        return true;
    }
    else if (board[0][2] == 1 && board[1][1] == 1 && board [2][0] == 1)
    {
        return true;
    }
    return false;
}

int Board::checkAIWin()
{
    if (board[0][0] == 2 && board[0][1] == 2 && board [0][2] == 2)
    {
        return true;
    }
    else if (board[1][0] == 2 && board[1][1] == 2 && board [1][2] == 2)
    {
        return true;
    }
    else if (board[2][0] == 2 && board[2][1] == 2 && board [2][2] == 2)
    {
        return true;
    }
    else if (board[0][0] == 2 && board[1][0] == 2 && board [2][0] == 2)
    {
        return true;
    }
    else if (board[0][1] == 2 && board[1][1] == 2 && board [2][1] == 2)
    {
        return true;
    }
    else if (board[0][2] == 2 && board[1][2] == 2 && board [2][2] == 2)
    {
        return true;
    }
    else if (board[0][0] == 2 && board[1][1] == 2 && board [2][2] == 2)
    {
        return true;
    }
    else if (board[0][2] == 2 && board[1][1] == 2 && board [2][0] == 2)
    {
        return true;
    }
    return false;
}

int Board::checkDraw()
{
    if (!gameOver() || checkP1Win() || checkAIWin())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Board::printBoard()
{
    for (int i=0; i<dimension; i++)
    {
        cout << endl << " ";
        for (int j=0; j<dimension; j++)
        {
            if (board[i][j]==0)
            {
                cout << " ";
            }
            else
            {
                cout << board[i][j];
            }
            if (j<2)
            {
                cout << " | ";
            }
        }
        if (i<2)
        {
            cout << endl << "-----------";
        }
    }
    cout << endl;
}

pair<int,int> Board::input()
{
    pair <int, int> inp;
    int r, c;
    cout << "Input row number (0 for first row): ";
    cin >> r;
    cout << "Input column number (0 for first column): ";
    cin >> c;
    if (r < dimension && r >= 0 && c < dimension && c >= 0)
    {
        inp = make_pair(r,c);
        return inp;
    }
    else
    {
    	cout << "Invalid pair" << endl;
    }
}

int Board::isValidMove(int row, int col)
{
    if (board[row][col] == 0 && row < dimension && row >= 0 && col < dimension && col >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

vector<pair<int, int> > Board::getValidMoves() //get list of all possible valid moves
{
    vector<pair<int, int> > val_mov;
    for (int i=0; i<dimension; i++)
    {
        for (int j=0; j<dimension; j++)
        {
            if (board[i][j] == 0)
            {
                val_mov.push_back(make_pair(i,j));
            }
        }
    }
    return val_mov;
}

void Board::setData(int r, int c, int num) //update a particular value on the board directly instead of making a move
{
    if (isValidMove(r,c))
    {
        board[r][c] = num;
    }
    else
    {
    	cout << "Invalid move." << endl;
    }

}

void Board::setPlayer(int x)
{
	currentPlayer = x;
}

bool Board::operator ==(const Board &rhs)
{
    if (dimension != rhs.dimension)
    {
        return false;
    }
    for (int i=0; i<dimension; i++)
    {
        for (int j=0; j<dimension; j++)
        {
            if (board[i][j] != rhs.board[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

void Board::operator = (const Board &lhs)
{
    this->dimension = lhs.dimension;
    this->currentPlayer = lhs.currentPlayer;
    for (int i=0; i<dimension; i++)
    {
        for (int j=0; j<dimension; j++)
        {
            board[i][j] = lhs.board[i][j];
        }
    }
}

int** Board::getPtr ()
{
    return board;
}

int Board::score()
{
    if (checkP1Win())
    {
        return 10;
    }
    else if (checkAIWin())
    {
        return -10;
    }
    else if (checkDraw())
    {
        return 0;
    }
}

bool Board::gameOver()
{
	if(checkP1Win() || checkAIWin())
    {
        return true;
    }
	bool incomplete = false;
	for(int i=0; i<dimension; i++)
    {
        for (int j=0; j<dimension; j++)
        {
            if (board[i][j]==0)
            {
                incomplete = true;
            }
        }
	}
	return !incomplete;
}

//Minimax Functions
pair<int,int> Board::minimax(int** samBoard)
{
	int topScore = 1000;
	pair<int, int> top_mov;
	for(int i=0; i<3; i++)
    {
		for(int j=0; j<3; j++)
		{
			if(samBoard[i][j] == 0)
			{
				samBoard[i][j] = 2; //AI 'tests' position i,j
				int temp_mov = maxSearch(samBoard);
				if(temp_mov <= topScore)
				{
					topScore = temp_mov;
					top_mov.first = i;
					top_mov.second = j;
				}
				samBoard[i][j] = 0; //unmark position once 'tested'
			}
		}
	}
	return top_mov;
}

int Board::maxSearch(int** samBoard)
{
	if(gameOver())
    {
        return score();
    }
	int topScore = -1000;
	for(int i=0; i<3; i++)
    {
		for(int j=0; j<3; j++)
		{
			if(samBoard[i][j] == 0)
			{
				samBoard[i][j] = 1; //player marks position
				int temp_mov = minSearch(samBoard);
				if(temp_mov >= topScore)
                {
					topScore = temp_mov;
				}
				samBoard[i][j] = 0; //position unmarked
			}
		}
	}
	return topScore;
}

int Board::minSearch (int** samBoard)
{
	if(gameOver())
    {
        return score();
    }
	int topScore = 1000;
	for(int i=0; i<3; i++)
    {
		for(int j=0; j<3; j++)
		{
			if(samBoard[i][j] == 0)
			{
				samBoard[i][j] = 2; //AI marks position
				int tempMove = maxSearch(samBoard);
				if(tempMove <= topScore)
				{
					topScore = tempMove;
				}
				samBoard[i][j] = 0; //position unmarked
			}
		}
	}
	return topScore;
}

int Board::playMinimax()
{
    cout << "Your game of TicTacToe against AI (Minimax Method) is about to begin." << endl;
    printBoard();
    while(!checkP1Win() && !checkAIWin() && !gameOver())
    {
        int r, c;
        cout << "Select row to make move (0 is first row): ";
        cin >> r;
        cout << "Select column to make move (0 is first column): ";
        cin >> c;
        makeMove(r,c);
        printBoard();
        if (checkP1Win())
        {
            cout << "You win. CONGRATS!" << endl;
            return 1;
        }
        else if (checkDraw())
        {
            cout << "Match drawn. WOW." << endl;
            return 0;
        }
        cout << "Computer will now take turn: " << endl;
        pair <int, int> aiMove = minimax(board);
        makeMove(aiMove.first, aiMove.second);
        printBoard();
        if (checkAIWin())
        {
            cout << "Computer wins. Try again." << endl;
            return 2;
        }
        else if (checkDraw())
        {
            cout << "Match drawn. WOW." << endl;
            return 0;
        }
    }
}