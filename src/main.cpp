#include "raylib.h"

#include "GameLoop.hpp"
#include "Constants.hpp"

int main()
{
	srand(time(NULL));

	InitWindow(SCREEN_WIDTH, GRIDE_HEIGHT, "🟥🟧🟨 TETRIX 🟩🟦🟪");

	SetTargetFPS(60);

	GameLoop game;
	game.initRandomly();

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);

		game.update();
		game.draw();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
