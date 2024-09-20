#pragma once

#include <vector>
#include <raylib.h>
#include <iostream>
#include "./Minos/DynamicMino.hpp"
#include "Constants.hpp"

class ShapeManager
{
	private:
		ShapeManager() {};
		~ShapeManager() {};
	
	public:
		static std::vector<DynamicMino*> getPiece(PieceType type);
};
