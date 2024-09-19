#pragma once

#include <iostream>

#include <raylib.h>
#include "../Constants.hpp"

class Mino
{
	private:
		Color color;

	public:
		Mino() : color(WHITE) {};
		Mino(Color color) : color(color) {}
		virtual ~Mino() {}

		Color getColor() { return color; }

		void draw(int cellX, int cellY);
};
