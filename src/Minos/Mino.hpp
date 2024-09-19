#pragma once

#include <raylib.h>
#include "../Constants.h"

class Mino
{
	private:
		Color color;

	public:
		Mino() : color(WHITE) {};
		Mino(Color color) : color(color) {}

		void draw(int cellX, int cellY);
};
