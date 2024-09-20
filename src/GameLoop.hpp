#pragma once

#include <raylib.h>

#include <vector>
#include <iostream>

#include "./Minos/Mino.hpp"
#include "./Minos/DynamicMino.hpp"
#include "./Minos/StaticMino.hpp"
#include "./ShapeManager.hpp"

class GameLoop
{
	private:
		std::vector<std::vector<Mino*>> grid;	// origin is top-left
		unsigned int frame = 0;

	public:
		GameLoop();
		~GameLoop();

		void draw();
		bool update();
		bool spawnNewTetromino();
		void gameOver();
};
