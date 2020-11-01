#include "board.h"
#include <vector>
using namespace std;


class Node {

  Board *data;
  vector<Node*> children;
  int score;
  public:
    Node();
    Node(Board* b1);
    void setData(Board *dta); //setter for data
    void addChild(Board *board);
    Board* getData();
    vector<Node*>* getChildren();
    void setScore(int a);
    int getScore();

  //Extra functions for convenience
  void removeChildren();
  vector<Node*> getChilds(); //access private variable. Return type vector instead of vector* as in getChildren()
};
