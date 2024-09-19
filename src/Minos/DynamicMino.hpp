#pragma once

#include "Mino.hpp"

// idk yet, pseudo so far

class DynamicMino : public Mino
{
	private:
		int pivotX, pivotY;

	public:
		DynamicMino(Color color, int pivotX, int pivotY) :
					Mino(color), pivotX(pivotX), pivotY(pivotY) {}
		bool movePossible(bool right);
		bool rotatePossible(bool clockwise);
};

/*
	TO ROTATE:
	1.	Calculate X and Y relative to pivot
	2.	new x = -y and new y = x for clockwise,
		new x = y and new y = -x for counterclockwise

	Move possibility checking & executing is seperated
	so GameLoop can verify if the move is possible for all minos
*/
