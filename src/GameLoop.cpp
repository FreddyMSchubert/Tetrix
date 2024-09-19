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
	for (size_t i = 0; i < grid.size() - 5; i++)
		for (size_t j = 0; j < grid[i].size(); j++)
			if (rand() % 3 == 0)
				grid[i][j] = new DynamicMino(DYNAMIC_COLOR, 0, 0);
	for (size_t i = grid.size() - 3; i < grid.size(); i++)
		for (size_t j = 0; j < grid[i].size(); j++)
			if (rand() % 3 == 0)
				grid[i][j] = new StaticMino(STATIC_COLOR);
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
		bool dynamicMinosDroppable = true;

		for (size_t i = grid.size() - 1; i != SIZE_MAX; i--) // stop once wraparound
		{
			for (size_t j = 0; j < grid[i].size(); j++)
			{
				DynamicMino *dm = dynamic_cast<DynamicMino*>(grid[i][j]);
				if (grid[i][j] == nullptr || dm == nullptr)
					continue;
				if (i == grid.size() - 1)
				{
					dynamicMinosDroppable = false;
					break;
				}
				DynamicMino *dmBelow = dynamic_cast<DynamicMino*>(grid[i + 1][j]);
				if (i == grid.size() - 1 || (grid[i + 1][j] != nullptr && dmBelow == nullptr))
				{
					dynamicMinosDroppable = false;
					break;
				}
			}
			if (!dynamicMinosDroppable)
				break;
		}
		if (dynamicMinosDroppable)
		{
			for (size_t i = grid.size() - 2; i != SIZE_MAX; i--) // stop once wraparound
			{
				for (size_t j = 0; j < grid[i].size(); j++)
				{
					if (grid[i][j] != nullptr && dynamic_cast<DynamicMino*>(grid[i][j]) == nullptr)
						continue;
					if (grid[i][j] != nullptr && grid[i + 1][j] == nullptr)
					{
						grid[i + 1][j] = grid[i][j];
						grid[i][j] = nullptr;
					}
				}
			}
		}
		else
		{
			for (size_t i = 0; i < grid.size(); i++)
				for (size_t j = 0; j < grid[i].size(); j++)
					if (grid[i][j] != nullptr)
						grid[i][j] = new StaticMino(STATIC_COLOR);
		}
	}
}
