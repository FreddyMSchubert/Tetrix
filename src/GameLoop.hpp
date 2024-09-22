#pragma once

#include <raylib.h>

#include <vector>
#include <iostream>

#include "./Mino.hpp"
#include "./ShapeManager.hpp"
#include "./Constants.hpp"
#include "./Structs.hpp"

typedef struct InputState {
	bool rightPressedLastFrame = false;		// move right
	bool leftPressedLastFrame = false;		// move left
	bool downPressedLastFrame = false;		// drop faster
	bool upPressedLastFrame = false;		// rotate
	bool spacePressedLastFrame = false;		// hard drop
	bool aPressedLastFrame = false;			// rotate left
	bool dPressedLastFrame = false;			// rotate right
	bool shiftPressedLastFrame = false;		// rotate right
} InputState;

class GameLoop
{
	private:
		std::vector<std::vector<Mino*>> grid;	// origin is top-left
		std::vector<PieceType> upcomingPieces;
		std::vector<PieceType> bag;
		PieceType *heldPiece = nullptr;
		PieceType currentPiece;
		unsigned int nextUpdate = DROP_SPEED;
		InputState inputState;
		GridPos pivot;

		unsigned int level = 0;
		unsigned int score = 0;
		unsigned int linesCleared = 0;
		float speed = 1.0f;

	public:
		GameLoop();
		~GameLoop();

		void draw();
		bool update();
		bool spawnNewTetromino();
		PieceType pickNewPiece();
		void holdPiece();
		bool moveDynamicMinos(int right, int down);
		bool rotateDynamicMinos(bool clockwise);
		bool deleteDynamicMinos();
		GridPos getRotationAroundPivot(GridPos pos, bool clockwise);
};
