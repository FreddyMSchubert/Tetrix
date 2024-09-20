#include "GameLoop.hpp"

GameLoop::GameLoop()
{
	grid = std::vector<std::vector<Mino*>>(CELLS_Y, std::vector<Mino*>(CELLS_X, nullptr));
}
GameLoop::~GameLoop()
{
	for (auto& row : grid)
		for (auto& cell : row)
			if (cell != nullptr)
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

	// Input handling
	if (!IsKeyPressed(KEY_RIGHT) && inputState.rightPressedLastFrame)
		moveDynamicMinos(1, 0);
	if (!IsKeyPressed(KEY_LEFT) && inputState.leftPressedLastFrame)
		moveDynamicMinos(-1, 0);

	inputState.rightPressedLastFrame = IsKeyPressed(KEY_RIGHT);
	inputState.leftPressedLastFrame = IsKeyPressed(KEY_LEFT);
	inputState.downPressedLastFrame = IsKeyPressed(KEY_DOWN);
	inputState.upPressedLastFrame = IsKeyPressed(KEY_UP);
	inputState.spacePressedLastFrame = IsKeyPressed(KEY_SPACE);

	// Update grid state
	if (frame % DROP_SPEED == 0)
	{

		// Try to drop, if nothing changed, turn dynamics static
		if (!moveDynamicMinos(0, 1))
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

// Returns whether anything changed
// input should be (-1, 1) to move it left and down
bool GameLoop::moveDynamicMinos(int right, int down)
{
	bool changeOccurred = false;

	// 1. move horizontally
	if (right != 0)
	{
		// Check if minos can move
		bool dynamicMinosMovable = true;
		bool dynamicMinosPresent = false;

		for (size_t i = 0; i < grid.size(); i++)
		{
			for (size_t j = 0; j < grid[i].size(); j++)
			{
				DynamicMino *dm = dynamic_cast<DynamicMino*>(grid[i][j]);
				if (grid[i][j] == nullptr || dm == nullptr)
					continue;
				dynamicMinosPresent = true;
				if (j + right < 0 || j + right >= grid[i].size())
				{
					dynamicMinosMovable = false;
					break;
				}
				DynamicMino *dmGoal = dynamic_cast<DynamicMino*>(grid[i][j + right]);
				if (grid[i][j + right] != nullptr && dmGoal == nullptr)
				{
					dynamicMinosMovable = false;
					break;
				}
			}
			if (!dynamicMinosMovable)
				break;
		}

		// Move minos
		size_t start = right > 0 ? grid[0].size() - 1 : 0;
		size_t end = right > 0 ? -1 : grid[0].size();
		size_t step = right > 0 ? -1 : 1;
		if (dynamicMinosMovable && dynamicMinosPresent)
		{
			for (size_t i = 0; i < grid.size(); i++)
			{
				for (size_t j = start; j != end; j += step)
				{
					if (grid[i][j] == nullptr)
						continue;
					DynamicMino *dm = dynamic_cast<DynamicMino*>(grid[i][j]);
					if (dm == nullptr)
						continue;
					if (grid[i][j + right] == nullptr)
					{
						grid[i][j + right] = grid[i][j];
						grid[i][j] = nullptr;
						changeOccurred = true;
					}
				}
			}
		}
	}

	// 2. move vertically
	if (down != 0)
	{
		// Check if minos can move
		bool dynamicMinosMovable = true;
		bool dynamicMinosPresent = false;
		for (size_t i = 0; i < grid.size(); i++)
		{
			for (size_t j = 0; j < grid[i].size(); j++)
			{
				DynamicMino *dm = dynamic_cast<DynamicMino*>(grid[i][j]);
				if (grid[i][j] == nullptr || dm == nullptr)
					continue;
				dynamicMinosPresent = true;
				if (i + down < 0 || i + down >= grid.size())
				{
					dynamicMinosMovable = false;
					break;
				}
				DynamicMino *dmGoal = dynamic_cast<DynamicMino*>(grid[i + down][j]);
				if (grid[i + down][j] != nullptr && dmGoal == nullptr)
				{
					dynamicMinosMovable = false;
					break;
				}
			}
			if (!dynamicMinosMovable)
				break;
		}

		// Move minos
		size_t start = down > 0 ? grid.size() - 1 : 0;
		size_t end = down > 0 ? -1 : grid.size();
		size_t step = down > 0 ? -1 : 1;
		if (dynamicMinosMovable && dynamicMinosPresent)
		{
			for (size_t i = start; i != end; i += step)
			{
				for (size_t j = 0; j < grid[i].size(); j++)
				{
					if (grid[i][j] == nullptr)
						continue;
					DynamicMino *dm = dynamic_cast<DynamicMino*>(grid[i][j]);
					if (dm == nullptr)
						continue;
					if (grid[i + down][j] == nullptr)
					{
						grid[i + down][j] = grid[i][j];
						grid[i][j] = nullptr;
						changeOccurred = true;
					}
				}
			}
		}
	}

	return changeOccurred;
}
