#include "Mino.hpp"

void Mino::draw(int cellX, int cellY)
{
	DrawRectangle(cellX * CELL_SIZE, cellY * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
}
