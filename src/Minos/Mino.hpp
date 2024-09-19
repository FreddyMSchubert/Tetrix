#pragma once

#include <raylib.h>

class Mino
{
	private:
		Color color;

	public:
		Mino() = default;
		Mino(Color color) : color(color) {}

		void draw(int cellX, int cellY);
};
