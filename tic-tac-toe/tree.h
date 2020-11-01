#include <string>
#include "node.h"
#include <iostream>

using namespace std;


class Tree {
  Node *root;
  Node *currentState; //pointer to node in tree with current state
  public:
    Tree();
    int generateTicTacToe(Node *head, int currentDepth, int totalDepth); //part 2
    void generateTicTacToe(int depth); // part 2
    void initCurrentState();
    Node* findNode(Node* ptr, Board b); //updateCurrentState helper
    void updateCurrentState(Board& b);
    void printCurrentState();
    Board* getCurrentState();
    void makeMove(int ROW, int COL); //move made by AI
    pair <int, int> highScoringMove(int turnNumber); //return move with highest node score
    int playScoring(); //play game based on our scoring
};

