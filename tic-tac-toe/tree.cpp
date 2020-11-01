#include <cmath>
//#include <ctime>
//#include <cstdlib>
#include "tree.h"

using namespace std;

Tree::Tree()
{
    root = new Node;
    currentState = root;
}

int Tree::generateTicTacToe(Node *head, int currentDepth, int totalDepth) //part 2
{
	//srand(time(0));
    currentState = head;
    //game finishes before reaching potential leaf node
    if (currentState->getData()->checkAIWin() == true || currentState->getData()->checkP1Win()==true)
    {
    	//higher payoffs for winning early. lower payoffs for losing early
    	if (currentState->getData()->checkAIWin() == true)
        {
            currentState->setScore(20); //AI wants to win - positive score on AI winning
        }
        else if (currentState->getData()->checkP1Win()==true)
        {
            currentState->setScore(-30);
        }
        //when not to display procedure comment print function
        /*printCurrentState();
        cout << "Score: " << currentState->getScore();
        cout << endl << endl;*/
        return currentState->getScore();
    }
    if (currentDepth == totalDepth) //base case
    {
    	//if score being assigned on full nodes only. Cater for both players winning on leaf node
    	/*if (currentState->getData()->checkP1Win()==true && currentState->getData()->checkAIWin() == true) 
        {
            int num = rand()%2;
            if (num==0)
                currentState->setScore(-20);
            if (num==1)
                currentState->setScore(10);
        }*/
        if (currentState->getData()->checkAIWin() == true)
        {
            currentState->setScore(10); //AI wants to win - positive score on AI winning
        }
        else if (currentState->getData()->checkP1Win()==true)
        {
            currentState->setScore(-20);
        }
        else if (currentState->getData()->checkDraw() == true)
        {
            currentState->setScore(0);
        }
        //when not to display procedure comment print function
        /*printCurrentState();
        cout << "Score: " << currentState->getScore();
        cout << endl << endl;*/
        return currentState->getScore();
    }

    else
    {
        vector<pair<int, int> > allMoves = currentState->getData()->getValidMoves(); //all positions to mark and make as children
        for (int i=0; i<allMoves.size(); i++)
        {
            Board* original = new Board(*(currentState->getData()));  //make copy of original board
            original->makeMove(allMoves[i].first, allMoves[i].second); //mark the extra coordinate - new child
            currentState->addChild(original);
        } //all children of currentState made and added as children
        vector<Node*>* newHead = currentState->getChildren();
        int scoreParent = 0;
        Node* saveIt = currentState; //to print nodes later (order: leaf nodes to root) with added score
        //printCurrentState(); //to see plan of action
        for (int i=0; i<(*newHead).size(); i++)
        {
            int score = generateTicTacToe((*newHead)[i], currentDepth+1, totalDepth);
            scoreParent += score;
        }
        currentState = saveIt;
        currentState->setScore(scoreParent); 
        //Comment printing if only head score needed. Uncomment to see score of all nodes
        /*printCurrentState();
        cout << "Score: " << currentState->getScore() << endl;*/
        return currentState->getScore();
    }
}

void Tree::generateTicTacToe(int depth)
{
	printCurrentState();
	cout << "Score: " << generateTicTacToe(currentState, 0, depth);
}

void Tree::initCurrentState()
{
	currentState = root;
	cout << "Initial currentState: " << endl;
	printCurrentState();
}

Node* Tree::findNode(Node* ptr, Board b)
{
    if(ptr == NULL)
    {
        return 0;
    }
    if(*(ptr->getData()) == b)
    {
        return ptr;
    }
    vector<Node*>* children = ptr->getChildren();
    for (int i=0; i<(*children).size(); i++)
    {
        Node* target = findNode((*children)[i], b);
        return target;
    }
    return 0;
}

void Tree::updateCurrentState(Board& b)
{
    Node* found = findNode(currentState, b);
    currentState = found;
    printCurrentState();
}

void Tree::printCurrentState()
{
	currentState->getData()->printBoard();
}

Board* Tree::getCurrentState()
{
	return currentState->getData();
}

void Tree::makeMove(int ROW, int COL)
{
	currentState->getData()->makeMove(ROW, COL);
}

pair <int, int> Tree::highScoringMove(int turnNumber)
{
    int topScore = -100000;
    Node* bestChild;
    pair <int, int> highScoreMove;
    vector<pair<int, int> > allMoves = currentState->getData()->getValidMoves();
    Node* tempState = new Node(currentState->getData()); //copy made so that ammendments not made to original tree
    tempState->removeChildren(); //children of node were made earlier. Necessary to remove them otherwise additional children would be tested
    Node* store = currentState; //initial currentState saved
    for (int i=0; i<allMoves.size(); i++)
    {
        Board* original = new Board(*(tempState->getData()));  
        original->makeMove(allMoves[i].first, allMoves[i].second);
        tempState->addChild(original);
    } //all children of tempState made and added as children
    vector<Node*> child = tempState->getChilds();
    for (int i=0; i<child.size(); i++)
    {
        int score = generateTicTacToe(child[i], turnNumber+1 , 9); //Player always taking first turn, so turnNumber+1 for AI
        if (score>=topScore)
        {
            topScore = score;
            bestChild = child[i];
        }
    } //bestChild retrieved. Get best move now by comparison of bestChild with parent
    currentState = store;
    for (int i=0; i<3; i++) //dimension = 3 
    {
        for (int j=0; j<3; j++)
        {
            if (bestChild->getData()->getPtr()[i][j] != currentState->getData()->getPtr()[i][j])
            {
                highScoreMove.first = i;
                highScoreMove.second = j;
            }
        }
    }
    return highScoreMove;
}

int Tree::playScoring()
{
	cout << "Your game of TicTacToe against AI (Scoring Nodes Method) is about to begin." << endl;
    int turn = 0;
    printCurrentState();
    while(!(currentState->getData()->checkP1Win()) && !(currentState->getData()->checkAIWin()) && !(currentState->getData()->gameOver()))
    {
        int r, c;
        cout << "Select row to make move (input 0 for first row): ";
        cin >> r;
        cout << "Select column to make move (input 0 for first column): ";
        cin >> c;
        makeMove(r, c);
        printCurrentState();
        if (currentState->getData()->checkP1Win())
        {
            cout << "You win. CONGRATS!" << endl;
            return 1;
        }
		else if (currentState->getData()->checkDraw())
        {
            cout << "Match drawn. WOW." << endl;
            return 0;
        }

        turn++;
        
        cout << endl << "Computer makes the move now... " << endl;
        pair <int, int> aiMove = highScoringMove(turn);
        makeMove(aiMove.first, aiMove.second);
        printCurrentState();
        turn++;
        if (currentState->getData()->checkAIWin())
        {
            cout << "Computer wins. Try again." << endl;
            return 2;
        }
        else if (currentState->getData()->checkDraw())
        {
            cout << "Match drawn. WOW." << endl;
            return 0;
        }
    }
}