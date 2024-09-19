#pragma once

#include "Mino.hpp"
#include "DynamicMino.hpp"

// eventually this will contain line clearing animation logic
// for now it's just a placeholder

class StaticMino : public Mino
{
	public:
		StaticMino(Color color) : Mino(color) {}
		StaticMino(DynamicMino mino) : Mino(mino.getColor()) {}
};
