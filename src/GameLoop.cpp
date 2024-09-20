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

void GameLoop::draw()
{
	for (size_t i = 0; i < grid.size(); i++)
		for (size_t j = 0; j < grid[i].size(); j++)
			if (grid[i][j] != nullptr)
				grid[i][j]->draw(j, i);
}

bool GameLoop::update()
{
	frame++;

	if (frame % DROP_SPEED == 0)
	{
		bool dynamicMinosDroppable = true;
		bool dynamicMinosPresent = false;

		// Check if minos can drop
		for (size_t i = grid.size() - 1; i != SIZE_MAX; i--)
		{
			for (size_t j = 0; j < grid[i].size(); j++)
			{
				DynamicMino *dm = dynamic_cast<DynamicMino*>(grid[i][j]);
				if (grid[i][j] == nullptr || dm == nullptr)
					continue;
				dynamicMinosPresent = true;
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
		
		// Drop minos
		if (dynamicMinosDroppable && dynamicMinosPresent)
		{
			for (size_t i = grid.size() - 2; i != SIZE_MAX; i--)
			{
				for (size_t j = 0; j < grid[i].size(); j++)
				{
					DynamicMino *dm = dynamic_cast<DynamicMino*>(grid[i][j]);
					if (grid[i][j] != nullptr && dm == nullptr)
						continue;
					if (grid[i][j] != nullptr && grid[i + 1][j] == nullptr)
					{
						grid[i + 1][j] = grid[i][j];
						grid[i][j] = nullptr;
					}
				}
			}
		}

		// If minos can't drop, make them static
		else
		{
			for (size_t i = 0; i < grid.size(); i++)
			{
				for (size_t j = 0; j < grid[i].size(); j++)
				{
					if (grid[i][j] != nullptr)
					{
						DynamicMino* dm = dynamic_cast<DynamicMino*>(grid[i][j]);
						if (dm == nullptr)
							continue;
						grid[i][j] = new StaticMino(dm->getColor());
						delete dm;
					}
				}
			}

			// Check for full rows
			for (size_t i = 0; i < grid.size(); i++)
			{
				bool rowFull = true;
				for (size_t j = 0; j < grid[i].size(); j++)
				{
					if (grid[i][j] == nullptr)
					{
						rowFull = false;
						break;
					}
				}
				if (rowFull)
				{
					for (size_t j = 0; j < grid[i].size(); j++)
					{
						delete grid[i][j];
						grid[i][j] = nullptr;
					}
					for (size_t k = i; k != 0; k--)
					{
						for (size_t j = 0; j < grid[k].size(); j++)
						{
							grid[k][j] = grid[k - 1][j];
							grid[k - 1][j] = nullptr;
						}
					}
				}
			}
		
			// then, spawn in a new piece
			if (!spawnNewTetromino())
				return false;
		}
	}

	return true;
}

bool GameLoop::spawnNewTetromino()
{
	PieceType type = static_cast<PieceType>(rand() % 7);
	std::vector<DynamicMino*> piece = ShapeManager::getPiece(type);
	int spawningOffset = (CELLS_X - 4) / 2;

	for (size_t i = 0; i < piece.size(); i++)
	{
		if (piece[i] == nullptr)
			continue;
		if (grid[i / 4][(i % 4) + spawningOffset] != nullptr)
		{
			gameOver();
			return false;
		}
		grid[i / 4][(i % 4) + spawningOffset] = piece[i];
	}

	return true;
}

void GameLoop::gameOver()
{
	std::cout << "💀💀💀 Game Over! 💀💀💀" << std::endl;
}
