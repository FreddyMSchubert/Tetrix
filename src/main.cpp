#include "raylib.h"

#define CELL_SIZE 25
#define GRIDE_WIDTH CELL_SIZE * 10
#define GRIDE_HEIGHT CELL_SIZE * 20
#define SIDEBAR_WIDTH 100
#define SCREEN_WIDTH GRIDE_WIDTH + SIDEBAR_WIDTH
#define SCREEN_HEIGHT GRIDE_HEIGHT

int main()
{
	InitWindow(SCREEN_WIDTH, GRIDE_HEIGHT, "window");

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);
		DrawText("Hello World!", 25, 200, 20, LIGHTGRAY);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
