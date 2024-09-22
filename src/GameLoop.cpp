#include "GameLoop.hpp"

GameLoop::GameLoop()
{
	grid = std::vector<std::vector<Mino*>>(CELLS_Y, std::vector<Mino*>(CELLS_X, nullptr));

	for (size_t i = 0; i < MAX_PIECE_QUEUE; i++)
		upcomingPieces.push_back(pickNewPiece());
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
	// draw grid
	for (int i = 0; i < CELLS_X; ++i)
	{
		for (int j = 0; j < CELLS_Y; ++j)
		{
			int x = i * CELL_SIZE;
			int y = j * CELL_SIZE;
			DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, GRID_COLOR);
		}
	}

	// draw minos
	for (size_t i = 0; i < grid.size(); i++)
		for (size_t j = 0; j < grid[i].size(); j++)
			if (grid[i][j] != nullptr)
				DrawRectangle(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE, grid[i][j]->getColor());

	// draw progress
	int leftOffset = grid[0].size() * CELL_SIZE + 5;
	DrawText(("LVL " + std::to_string(level)).c_str(), leftOffset, 20, 15, WHITE);
	DrawText(("SCR " + std::to_string(score)).c_str(), leftOffset, 35, 15, WHITE);
	DrawText(("LNS " + std::to_string(linesCleared)).c_str(), leftOffset, 50, 15, WHITE);

	// draw upcoming pieces
	int margin = 10;
	int pieceSize = 4 * CELL_SIZE;
	for (size_t i = 0; i < PIECE_LOOKAHEAD; i++)
	{
		GridPos pos;
		std::vector<Mino*> piece = ShapeManager::getPiece(upcomingPieces[i], pos);
		for (size_t j = 0; j < piece.size(); j++)
		{
			if (piece[j] == nullptr)
				continue;

			int x = grid[0].size() * CELL_SIZE + margin + (j % 4) * CELL_SIZE;
			int y = margin + (i * pieceSize) + (j / 4) * CELL_SIZE + (pieceSize * 2);
			DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, piece[j]->getColor());
		}
		for (size_t j = 0; j < piece.size(); j++)
			if (piece[j] != nullptr)
				delete piece[j];
	}
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
	if (!IsKeyPressed(KEY_LEFT_SHIFT) && inputState.shiftPressedLastFrame)
		holdPiece();

	inputState.rightPressedLastFrame = IsKeyPressed(KEY_RIGHT);
	inputState.leftPressedLastFrame = IsKeyPressed(KEY_LEFT);
	inputState.downPressedLastFrame = IsKeyPressed(KEY_DOWN);
	inputState.upPressedLastFrame = IsKeyPressed(KEY_UP);
	inputState.spacePressedLastFrame = IsKeyPressed(KEY_SPACE);
	inputState.aPressedLastFrame = IsKeyPressed(KEY_A);
	inputState.dPressedLastFrame = IsKeyPressed(KEY_D);
	inputState.shiftPressedLastFrame = IsKeyPressed(KEY_LEFT_SHIFT);

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
					grid[i][j]->setIsDynamic(false);
				}
			}

			// Check for full rows
			int clearedRows = 0;
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
					clearedRows++;
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
			linesCleared += clearedRows;
			unsigned int prevLevel = level;
			level = linesCleared / 10;
			if (level != prevLevel)
				nextUpdate = DROP_SPEED / (level * LEVEL_UP_SPEED_INCREASE);
			if (clearedRows > 0)
			{
				switch (clearedRows)
				{
					case 1:
						score += 40 * (level + 1);
						break;
					case 2:
						score += 100 * (level + 1);
						break;
					case 3:
						score += 300 * (level + 1);
						break;
					case 4:
						score += 1200 * (level + 1);
						break;
					default:
						std::cout << "What? How did you clear something other than 0-4 rows?" << std::endl;
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
	PieceType type = upcomingPieces[0];
	upcomingPieces.erase(upcomingPieces.begin());
	if (upcomingPieces.size() < PIECE_LOOKAHEAD)
	{
		for (size_t i = upcomingPieces.size(); i < PIECE_LOOKAHEAD; i++)
			upcomingPieces.push_back(pickNewPiece());
	}
	currentPiece = type;

	std::vector<Mino*> piece = ShapeManager::getPiece(type, pivot);
	int spawningOffset = (CELLS_X - 4) / 2;

	pivot.x += spawningOffset;
	for (size_t i = 0; i < piece.size(); i++)
	{
		if (piece[i] == nullptr)
			continue;
		if (grid[i / 4][(i % 4) + spawningOffset] != nullptr)
		{
			std::cout << "💀💀💀 Game Over! 💀💀💀" << std::endl;
			return false;
		}
		grid[i / 4][(i % 4) + spawningOffset] = piece[i];
	}

	return true;
}

PieceType GameLoop::pickNewPiece()
{
	if (bag.empty())
		bag = ShapeManager::getBag();
	int pieceIndex = rand() % bag.size();
	PieceType piece = bag[pieceIndex];
	bag.erase(bag.begin() + pieceIndex);
	return piece;
}

void GameLoop::holdPiece()
{
	if (heldPiece == nullptr)
	{
		heldPiece = new PieceType(currentPiece);
		deleteDynamicMinos();
		spawnNewTetromino();
	}
	else
	{
		PieceType nextPiece = *heldPiece;
		heldPiece = new PieceType(currentPiece);
		deleteDynamicMinos();
		upcomingPieces.insert(upcomingPieces.begin(), nextPiece);
		spawnNewTetromino();
	}
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

bool GameLoop::deleteDynamicMinos()
{
	bool changeOccurred = false;
	for (size_t j = 0; j < grid.size(); j++)
	{
		for (size_t i = 0; i < grid[j].size(); i++)
		{
			if (grid[j][i] == nullptr)
				continue;
			Mino *dm = dynamic_cast<Mino*>(grid[j][i]);
			if (dm == nullptr || !dm->getIsDynamic())
				continue;
			delete grid[j][i];
			grid[j][i] = nullptr;
			changeOccurred = true;
		}
	}
	return changeOccurred;
}
