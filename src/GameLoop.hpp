#pragma once

#include <raylib.h>

#include <vector>
#include <iostream>

#include "./Minos/Mino.hpp"
#include "./Minos/DynamicMino.hpp"
#include "./Minos/StaticMino.hpp"
#include "./ShapeManager.hpp"

typedef struct InputState {
	bool rightPressedLastFrame = false;		// move right
	bool leftPressedLastFrame = false;		// move left
	bool downPressedLastFrame = false;		// drop faster
	bool upPressedLastFrame = false;		// rotate
	bool spacePressedLastFrame = false;		// hard drop
} InputState;

class GameLoop
{
	private:
		std::vector<std::vector<Mino*>> grid;	// origin is top-left
		unsigned int nextUpdate = DROP_SPEED;
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
