#include "node.h"

using namespace std;

Node::Node()
{
    data = new Board();
}

Node::Node(Board* b1)
{
    data = b1;
}

void Node::setData(Board *dta)
{
    *data = *dta;
}

void Node::addChild(Board *board)
{
    Node* n1 = new Node(board);
    children.push_back(n1);
}

Board* Node::getData()
{
    return data;
}

vector<Node*>* Node::getChildren()
{
    vector<Node*>* childV = &children;
    return childV;
}

void Node::setScore(int a)
{
    score = a;
}

int Node::getScore()
{
    return score;
}


//Extra functions for convenience
void Node::removeChildren() //To be used in play function based on node scores
{
    for (int i=0; i<children.size(); i++)
    {
        children.pop_back();
    }
}

vector<Node*> Node::getChilds() //access private variable. Return type vector instead of vector* as in getChildren()
{
    return children;
}