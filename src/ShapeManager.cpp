#include "ShapeManager.hpp"

std::vector<Mino*> ShapeManager::getPiece(PieceType type, GridPos &Pivot)
{
	std::vector<Mino*> piece(8, nullptr);
	switch (type)
	{
		case PieceType::I:
			Pivot = {0, 1};
			piece = {new Mino(I_PIECE_COLOR, true), new Mino(I_PIECE_COLOR, true), new Mino(I_PIECE_COLOR, true), new Mino(I_PIECE_COLOR, true)};
			break;
		case PieceType::J:
			Pivot = {1, 1};
			piece = {new Mino(J_PIECE_COLOR, true), nullptr, nullptr, nullptr,
					new Mino(J_PIECE_COLOR, true), new Mino(J_PIECE_COLOR, true), new Mino(J_PIECE_COLOR, true), nullptr};
			break;
		case PieceType::L:
			Pivot = {1, 1};
			piece = {nullptr, nullptr, new Mino(L_PIECE_COLOR, true), nullptr,
					new Mino(L_PIECE_COLOR, true), new Mino(L_PIECE_COLOR, true), new Mino(L_PIECE_COLOR, true), nullptr};
			break;
		case PieceType::O:
			Pivot = {0, 0};
			piece = {nullptr, new Mino(O_PIECE_COLOR, true), new Mino(O_PIECE_COLOR, true), nullptr,
					nullptr, new Mino(O_PIECE_COLOR, true), new Mino(O_PIECE_COLOR, true), nullptr};
			break;
		case PieceType::T:
			Pivot = {1, 1};
			piece = {nullptr, new Mino(T_PIECE_COLOR, true), nullptr, nullptr,
					new Mino(T_PIECE_COLOR, true), new Mino(T_PIECE_COLOR, true), new Mino(T_PIECE_COLOR, true), nullptr};
			break;
		case PieceType::S:
			Pivot = {1, 1};
			piece = {nullptr, new Mino(S_PIECE_COLOR, true), new Mino(S_PIECE_COLOR, true), nullptr,
					new Mino(S_PIECE_COLOR, true), new Mino(S_PIECE_COLOR, true), nullptr, nullptr};
			break;
		case PieceType::Z:
			Pivot = {1, 1};
			piece = {new Mino(Z_PIECE_COLOR, true), new Mino(Z_PIECE_COLOR, true), nullptr, nullptr,
					nullptr, new Mino(Z_PIECE_COLOR, true), new Mino(Z_PIECE_COLOR, true), nullptr};
			break;
	}
	return piece;
}
