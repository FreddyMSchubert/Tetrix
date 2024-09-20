#include "ShapeManager.hpp"

std::vector<DynamicMino*> ShapeManager::getPiece(PieceType type)
{
	switch (type)
	{
		case PieceType::I:
			return	{new DynamicMino(I_PIECE_COLOR, 2, 1), new DynamicMino(I_PIECE_COLOR, 1, 1), new DynamicMino(I_PIECE_COLOR, 0, 1), new DynamicMino(I_PIECE_COLOR, -1, 1),
					nullptr, nullptr, nullptr, nullptr,
					nullptr, nullptr, nullptr, nullptr,
					nullptr, nullptr, nullptr, nullptr};
		case PieceType::J:
			return	{new DynamicMino(J_PIECE_COLOR, 1.5f, 1.5f), nullptr, nullptr, nullptr,
					new DynamicMino(J_PIECE_COLOR, 1.5f, 0.5f), new DynamicMino(J_PIECE_COLOR, 0.5f, 0.5f), new DynamicMino(J_PIECE_COLOR, -0.5f, 0.5f), nullptr,
					nullptr, nullptr, nullptr, nullptr,
					nullptr, nullptr, nullptr, nullptr};
		case PieceType::L:
			return	{nullptr, nullptr, new DynamicMino(L_PIECE_COLOR, -0.5f, 1.5f), nullptr,
					new DynamicMino(L_PIECE_COLOR, 1.5f, 0.5f), new DynamicMino(L_PIECE_COLOR, 0.5f, 0.5f), new DynamicMino(L_PIECE_COLOR, -0.5f, 0.5f), nullptr,
					nullptr, nullptr, nullptr, nullptr,
					nullptr, nullptr, nullptr, nullptr};
		case PieceType::O:
			return	{nullptr, new DynamicMino(O_PIECE_COLOR, 1, 1), new DynamicMino(O_PIECE_COLOR, 0, 1), nullptr,
					nullptr, new DynamicMino(O_PIECE_COLOR, 1, 0), new DynamicMino(O_PIECE_COLOR, 0, 0), nullptr,
					nullptr, nullptr, nullptr, nullptr,
					nullptr, nullptr, nullptr, nullptr};
		case PieceType::T:
			return	{nullptr, new DynamicMino(T_PIECE_COLOR, 0.5f, 1.5f), nullptr, nullptr,
					new DynamicMino(T_PIECE_COLOR, 1.5f, 0.5f), new DynamicMino(T_PIECE_COLOR, 0.5f, 0.5f), new DynamicMino(T_PIECE_COLOR, -0.5f, 0.5f), nullptr,
					nullptr, nullptr, nullptr, nullptr,
					nullptr, nullptr, nullptr, nullptr};
		case PieceType::S:
			return	{nullptr, new DynamicMino(S_PIECE_COLOR, 0.5f, 1.5f), new DynamicMino(S_PIECE_COLOR, -0.5f, 1.5f), nullptr,
					new DynamicMino(S_PIECE_COLOR, 1.5f, 0.5f), new DynamicMino(S_PIECE_COLOR, 0.5f, 0.5f), nullptr, nullptr,
					nullptr, nullptr, nullptr, nullptr,
					nullptr, nullptr, nullptr, nullptr};
		case PieceType::Z:
			return	{new DynamicMino(Z_PIECE_COLOR, 1.5f, 1.5f), new DynamicMino(Z_PIECE_COLOR, 0.5f, 1.5f), nullptr, nullptr,
					nullptr, new DynamicMino(Z_PIECE_COLOR, 0.5f, 0.5f), new DynamicMino(Z_PIECE_COLOR, -0.5f, 0.5f), nullptr,
					nullptr, nullptr, nullptr, nullptr,
					nullptr, nullptr, nullptr, nullptr};
	}
}
