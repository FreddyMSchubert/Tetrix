#include "raylib.h"

#include "GameLoop.hpp"
#include "Constants.hpp"

int main()
{
	srand(time(NULL));

	InitWindow(SCREEN_WIDTH, GRIDE_HEIGHT, "🟥🟧🟨 TETRIX 🟩🟦🟪");

	SetTargetFPS(60);

	GameLoop game;

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);

		if (!game.update())
			break; // game over
		game.draw();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
