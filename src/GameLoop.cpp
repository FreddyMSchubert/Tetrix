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
				grid[i][j] = new Mino({static_cast<unsigned char>((rand() % 128) + 128),
										static_cast<unsigned char>((rand() % 128) + 128),
										static_cast<unsigned char>((rand() % 128) + 128),
										255});
}
void GameLoop::draw()
{
	for (size_t i = 0; i < grid.size(); i++)
		for (size_t j = 0; j < grid[i].size(); j++)
			if (grid[i][j] != nullptr)
				grid[i][j]->draw(j, i);
}

void GameLoop::update()
{
	frame++;

	if (frame % dropSpeed == 0)
	{
		for (size_t i = grid.size() - 2; i != SIZE_MAX; i--) // stop once wraparound
		{
			for (size_t j = 0; j < grid[i].size(); j++)
			{
				if (grid[i][j] != nullptr && grid[i + 1][j] == nullptr)
				{
					grid[i + 1][j] = grid[i][j];
					grid[i][j] = nullptr;
				}
			}
			std::cout << "Updating line " << i << " of " << grid.size() << std::endl;
		}
	}
}
