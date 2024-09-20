#pragma once

#include <raylib.h>

#include <vector>
#include <iostream>

#include "./Minos/Mino.hpp"
#include "./Minos/DynamicMino.hpp"
#include "./Minos/StaticMino.hpp"
#include "./ShapeManager.hpp"

typedef struct InputState {
	bool rightPressedLastFrame = false;
	bool leftPressedLastFrame = false;
	bool downPressedLastFrame = false;
	bool upPressedLastFrame = false;
	bool spacePressedLastFrame = false;
} InputState;

class GameLoop
{
	private:
		std::vector<std::vector<Mino*>> grid;	// origin is top-left
		unsigned int frame = 0;
		InputState inputState;

	public:
		GameLoop();
		~GameLoop();

		void draw();
		bool update();
		bool spawnNewTetromino();
		void gameOver();
		bool moveDynamicMinos(int right, int down);
};
