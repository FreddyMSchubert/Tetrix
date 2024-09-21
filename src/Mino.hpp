#pragma once

#include <iostream>

#include <raylib.h>
#include "./Constants.hpp"

class Mino
{
	private:
		Color color;
		bool isDynamic;

	public:
		Mino(Color color, bool isDynamic) : color(color), isDynamic(isDynamic) {}
		virtual ~Mino() {}

		Color getColor() { return color; }
		bool getIsDynamic() { return isDynamic; }
};
