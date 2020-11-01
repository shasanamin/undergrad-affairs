/*Limitations: A good amount of computation done even with 3x3 board. As we move to 4x4 and higher combinations, amount of computations being done
increase manifold (more than even thousands of time). This will tax you a lot of computing power. Ordinary computers can run out of memory on stack and/ or
heap causing program to crash when say dealing with 10x10 board. Even with 'stronger' computers, the processing will be significantly time consuming so game 
time would increase and play can get boring (think of AI taking half an hour for its first move). Recursion used in code which already is not optimal for
saving computing time/ power.*/
/*Possible Solutions: Eliminate redundant boards i.e. boards with exactly same configuration which do occur as children to different parents.
Remove boards that do not seem redundant by their first look but actually are -- 90 degree board rotations and flips about vertical, horizontal, and
diagonal axis are redundant. Make sure you do maximum work in heap otherwise program more prone to crash. Lastly, while it is significantly more difficult to
implement, find an iterative solution rather than recursive solution. Also, can use other algorithms like Negamax or Alpha-beta pruning*/


#include <iostream>
#include "tree.h"

using namespace std;

int main() 
{
	Tree t;
	t.playScoring();
	cout << endl;
	Board* empty = new Board();
	empty->playMinimax();
	return 0;
}
