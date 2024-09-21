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
				DrawRectangle(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE, grid[i][j]->getColor());
}

bool GameLoop::update()
{
	nextUpdate--;

	// Input handling
	if (!IsKeyPressed(KEY_RIGHT) && inputState.rightPressedLastFrame)
		moveDynamicMinos(1, 0);
	if (!IsKeyPressed(KEY_LEFT) && inputState.leftPressedLastFrame)
		moveDynamicMinos(-1, 0);
	if (IsKeyPressed(KEY_DOWN) && !inputState.downPressedLastFrame)
		nextUpdate = 0;
	if (!IsKeyPressed(KEY_SPACE) && inputState.spacePressedLastFrame)
		while (moveDynamicMinos(0, 1));
	if (!IsKeyPressed(KEY_UP) && inputState.upPressedLastFrame)
		rotateDynamicMinos(false);
	if (!IsKeyPressed(KEY_A) && inputState.aPressedLastFrame)
		rotateDynamicMinos(false);
	if (!IsKeyPressed(KEY_D) && inputState.dPressedLastFrame)
		rotateDynamicMinos(true);

	inputState.rightPressedLastFrame = IsKeyPressed(KEY_RIGHT);
	inputState.leftPressedLastFrame = IsKeyPressed(KEY_LEFT);
	inputState.downPressedLastFrame = IsKeyPressed(KEY_DOWN);
	inputState.upPressedLastFrame = IsKeyPressed(KEY_UP);
	inputState.spacePressedLastFrame = IsKeyPressed(KEY_SPACE);
	inputState.aPressedLastFrame = IsKeyPressed(KEY_A);
	inputState.dPressedLastFrame = IsKeyPressed(KEY_D);

	// Update grid state
	if (nextUpdate == 0)
	{
		nextUpdate = DROP_SPEED;
		if (IsKeyDown(KEY_DOWN))
			nextUpdate = SPED_UP_DROP_SPEED;

		// Try to drop, if nothing changed, turn dynamics static
		if (!moveDynamicMinos(0, 1))
		{
			for (size_t i = 0; i < grid.size(); i++)
			{
				for (size_t j = 0; j < grid[i].size(); j++)
				{
					if (grid[i][j] == nullptr)
						continue;
					Mino* dm = dynamic_cast<Mino*>(grid[i][j]);
					if (dm == nullptr || !dm->getIsDynamic())
						continue;
					grid[i][j] = new Mino(dm->getColor(), false);
					delete dm;
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
	std::vector<Mino*> piece = ShapeManager::getPiece(type, pivot);
	int spawningOffset = (CELLS_X - 4) / 2;

	pivot.x += spawningOffset;
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
				Mino *dm = dynamic_cast<Mino*>(grid[i][j]);
				if (grid[i][j] == nullptr || dm == nullptr || !dm->getIsDynamic())
					continue;
				dynamicMinosPresent = true;
				if (j + right < 0 || j + right >= grid[i].size())
				{
					dynamicMinosMovable = false;
					break;
				}
				if (grid[i][j + right] == nullptr)
					continue;
				Mino *dmGoal = dynamic_cast<Mino*>(grid[i][j + right]);
				if (!dmGoal->getIsDynamic())
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
			pivot.x += right;
			for (size_t i = 0; i < grid.size(); i++)
			{
				for (size_t j = start; j != end; j += step)
				{
					if (grid[i][j] == nullptr)
						continue;
					Mino *dm = dynamic_cast<Mino*>(grid[i][j]);
					if (!dm->getIsDynamic())
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
				Mino *dm = dynamic_cast<Mino*>(grid[i][j]);
				if (grid[i][j] == nullptr || dm == nullptr || !dm->getIsDynamic())
					continue;
				dynamicMinosPresent = true;
				if (i + down < 0 || i + down >= grid.size())
				{
					dynamicMinosMovable = false;
					break;
				}
				if (grid[i + down][j] == nullptr)
					continue;
				Mino *dmGoal = dynamic_cast<Mino*>(grid[i + down][j]);
				if (!dmGoal->getIsDynamic())
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
			pivot.y += down;
			for (size_t i = start; i != end; i += step)
			{
				for (size_t j = 0; j < grid[i].size(); j++)
				{
					if (grid[i][j] == nullptr)
						continue;
					Mino *dm = dynamic_cast<Mino*>(grid[i][j]);
					if (dm == nullptr || !dm->getIsDynamic())
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

bool GameLoop::rotateDynamicMinos(bool clockwise)
{
	std::vector<std::pair<Mino*, GridPos>> dynamicMinos;
	std::vector<std::pair<Mino*, GridPos>> newDynamicMinos;

	// 1. Get a vector of all dynamic minos
	for (size_t i = 0; i < grid.size(); i++)
	{
		for (size_t j = 0; j < grid[i].size(); j++)
		{
			Mino *dm = dynamic_cast<Mino*>(grid[i][j]);
			if (grid[i][j] == nullptr || dm == nullptr || !dm->getIsDynamic())
				continue;
			dynamicMinos.push_back({dm, {static_cast<int>(i), static_cast<int>(j)}});
		}
	}
	if (dynamicMinos.empty())
		return false;

	// 2. Check if minos can rotate
	for (size_t i = 0; i < dynamicMinos.size(); i++)
	{
		GridPos newPos = getRotationAroundPivot(dynamicMinos[i].second, clockwise);
		if (newPos.x < 0 || newPos.x >= static_cast<int>(grid[0].size()) || newPos.y < 0 || newPos.y >= static_cast<int>(grid.size()))
			return false;
		if (grid[newPos.y][newPos.x] != nullptr && !dynamic_cast<Mino*>(grid[newPos.y][newPos.x])->getIsDynamic())
			return false;
		newDynamicMinos.push_back({dynamicMinos[i].first, newPos});
	}

	// 3. Rotate minos
	for (size_t i = 0; i < dynamicMinos.size(); i++)
	{
		grid[dynamicMinos[i].second.y][dynamicMinos[i].second.x] = nullptr;
	}
	for (size_t i = 0; i < newDynamicMinos.size(); i++)
	{
		grid[newDynamicMinos[i].second.y][newDynamicMinos[i].second.x] = newDynamicMinos[i].first;
	}
	return !newDynamicMinos.empty();
}

GridPos GameLoop::getRotationAroundPivot(GridPos pos, bool clockwise)
{
	int relX = pos.x - pivot.x;
	int relY = pos.y - pivot.y;

	int newRelX, newRelY;

	if (clockwise)
	{
		newRelX = relY;
		newRelY = -relX;
	}
	else
	{
		newRelX = -relY;
		newRelY = relX;
	}

	GridPos newPos;
	newPos.x = newRelX + pivot.x;
	newPos.y = newRelY + pivot.y;

	return newPos;
}
