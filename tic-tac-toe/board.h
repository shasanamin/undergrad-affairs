#include <string>
#include <vector>

using namespace std;

class Board {

  //3x3 board
  int **board;
  int currentPlayer;
  
  int dimension; // dimensions of game. For our purpose, this will always be 3 as we are working with a 3x3 board

  public:
    Board();
    Board(int a);
    Board(Board&);
    void makeMove(int row, int col); // update the board
    int checkWin(); // checks if someone has won the game
    int checkP1Win(); //checks if P1 has won the game
    int checkAIWin(); //check if AI has won the game
    int checkDraw();
    void printBoard();
    pair<int,int> input(); //take input from user
    int isValidMove(int row, int col);
    vector<pair<int, int> > getValidMoves(); //get list of all possible valid moves
    void setData(int r, int c, int num); //update a particular value on the board directly instead of making a move
    void setPlayer(int a);
    bool operator ==(const Board &rhs);
    void operator = (const Board &lhs); 

    //Extra functions
    int** getPtr(); //to access int** board
    int score(); //scoring for minimax
    bool gameOver(); //checks if game has ended

    //Minimax functions
    //samBoard is same as board here but passing as argument for added flexibility of playing on various boards
    pair<int,int> minimax(int** samBoard);
    int maxSearch(int** samBoard);
    int minSearch (int** samBoard);
    int playMinimax();

};
