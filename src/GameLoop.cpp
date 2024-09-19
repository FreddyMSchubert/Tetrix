#include "GameLoop.hpp"

GameLoop::GameLoop()
{
	grid = std::vector<std::vector<Mino*>>(CELLS_Y, std::vector<Mino*>(CELLS_X, nullptr));
}
GameLoop::~GameLoop()
{
	for (auto& row : grid)
		for (auto& cell : row)
			delete cell;
}

void GameLoop::initRandomly()
{
	for (size_t i = 0; i < grid.size(); i++)
		for (size_t j = 0; j < grid[i].size(); j++)
			if (rand() % 3 == 0)
				grid[i][j] = new Mino({static_cast<unsigned char>(rand() % 256),
										static_cast<unsigned char>(rand() % 256),
										static_cast<unsigned char>(rand() % 256),
										255});
}
void GameLoop::draw()
{
	for (size_t i = 0; i < grid.size(); i++)
		for (size_t j = 0; j < grid[i].size(); j++)
			if (grid[i][j] != nullptr)
				grid[i][j]->draw(j, i);
}
