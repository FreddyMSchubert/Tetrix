#include "raylib.h"

#include "GameGrid.hpp"
#include "Constants.hpp"

int main()
{
	srand(time(NULL));

	InitWindow(SCREEN_WIDTH, GRIDE_HEIGHT, "🟥🟧🟨 TETRIX 🟩🟦🟪");

	SetTargetFPS(60);

	GameGrid grid;

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);

		if (!grid.update())
			break; // game over
		grid.draw();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
