#pragma once

#include <vector>
#include <raylib.h>
#include <iostream>
#include "Constants.hpp"
#include "Structs.hpp"
#include "Mino.hpp"

class ShapeManager
{
	private:
		ShapeManager() {};
		~ShapeManager() {};
	
	public:
		static std::vector<Mino*> getPiece(PieceType type, GridPos &Pivot);
		static std::vector<PieceType> getBag();
};
