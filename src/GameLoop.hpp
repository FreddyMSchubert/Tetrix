#pragma once

#include <raylib.h>
#include <vector>
#include "./Minos/Mino.hpp"

class GameLoop
{
	private:
		std::vector<std::vector<Mino*>> grid;

	public:
		GameLoop();
		~GameLoop();

		void initRandomly();
		void draw();
};