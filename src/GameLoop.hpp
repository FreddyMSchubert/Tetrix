#pragma once

#include <raylib.h>

#include <vector>
#include <iostream>

#include "./Minos/Mino.hpp"

class GameLoop
{
	private:
		std::vector<std::vector<Mino*>> grid;	// origin is top-left
		unsigned int frame = 0;
		unsigned char dropSpeed = 60;			// every X frames, drop minos by 1 cell

	public:
		GameLoop();
		~GameLoop();

		void initRandomly();
		void draw();
		void update();
};
