#ifndef CS240_POSITION_H
#define CS240_POSITION_H

#include <iostream>
//! Position is a value object which holds a single set of coordinates on the game board
class Position{
public:
	//! Constructor
	//! @args x and y coordinates of the board position
	Position(int x, int y);
	//@return an array with {row, col}
	int GetRow();
	int GetCol();
	bool operator< (Position& other);
	static bool Test(std::ostream& os);
private:
	int row; //! 0-7, starting on the left of the board
	int col; //! 0-7, starting at the top of the board.
};
#endif
