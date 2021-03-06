#ifndef CS240_SQUARE_H
#define CS240_SQUARE_H

#include "Position.h"
#include "Piece.h"

//! Square is a value object containing information on the state of each square on the board.
class Square{
public:
	//! Constructor with all the values needed for the class (piece is NULL if unoccupied
	Square(Position* pos, Piece* piece);

	//! Changes the occupant in the square
	void ChangeOccupant(Piece* newOccupant);
	//! Accessors for private data
	Position* GetPosition() const;
	Piece* GetOccupant() const;

	static bool Test(std::ostream& os);

private:
	Position* position; //! Location on the board
	Piece* occupant; //! Piece currently occupying the square



};

#endif
