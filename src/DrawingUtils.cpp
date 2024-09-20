#include "DrawingUtils.hpp"

void DrawingUtils::drawGrid()
{
	for (int i = 0; i < CELLS_X; ++i)
	{
		for (int j = 0; j < CELLS_Y; ++j)
		{
			int x = i * CELL_SIZE;
			int y = j * CELL_SIZE;
			DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, GRID_COLOR);
		}
	}
}
