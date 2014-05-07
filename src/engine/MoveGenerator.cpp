/*
 * MoveGenerator.cpp
 *
 *  Created on: 05.05.2014
 *      Author: Christian
 */

#include "MoveGenerator.h"
#include "../Globals.h"
#include "BoardBase.h"
#include "Board.h"

namespace eng
{

template<class BOARD_T>
void MoveGenerator<BOARD_T>::gen_moves(BOARD_T& board, std::vector<typename BOARD_T::GenMove_t>& moves)
{
	typename BOARD_T::Piece_t piece;
	typename BOARD_T::Color_t color;
	typename BOARD_T::Color_t c1;
	typename BOARD_T::Color_t c2;

	c1 = board.get_color();
	c2 = (c1 == BOARD_T::WHITE) ? BOARD_T::BLACK : BOARD_T::WHITE;

	for (int i = 0; i < 64; i++)
	{
		color = board.get_color(i);
		piece = board.get_piece(i);
		if (color == c1 && piece != BoardBase::EMPTY)
		{
			if (piece != BoardBase::PAWN)
			{

				for (int j = 0; j < BoardBase::_offsets[piece]; j++)
				{
					for (int n = i;;)
					{
						n = BoardBase::_mailbox[BoardBase::_mailbox64[n] + BoardBase::_offset[piece][j]];

						if (n == -1)
							break;

						if (board.get_piece(n) != BoardBase::EMPTY)
						{
							if (board.get_color(n) == c2)
							{
								moves.push_back(board.gen_mov(i, n));
							}
							break;
						}
						moves.push_back(board.gen_mov(i, n));

						if (!BoardBase::_slide[piece])
						{
							break;
						}
					}
				}
			}
			else /*PAWN*/
			{
				int32_t step;
				step = (c1 == BOARD_T::WHITE) ? 1 : -1;
				if (board.get_piece(i + step * 8) == BoardBase::EMPTY)
				{
					moves.push_back(board.gen_mov(i, i + step * 8));
					if (!board.has_moved(i) && board.get_piece(i + 2 * step * 8) == BoardBase::EMPTY)
					{
						moves.push_back(board.gen_mov(i, i + 2 * step));
					}
				}

				/*pawn capture*/
				if (board.get_piece(i + step + step * 8) != BoardBase::EMPTY)
				{
					if (board.get_color(i + step + step * 8) == c2)
					{
						moves.push_back(board.gen_mov(i, i + step + step * 8));
					}
				}

				if (board.get_piece(i - step + step * 8) != BoardBase::EMPTY)
				{
					if (board.get_color(i - step + step * 8) == c2)
					{
						moves.push_back(board.gen_mov(i, i - step + step * 8));
					}
				}

				/* TODO missing moves: en passant, promoting, castle */

			}
		}
	}
}

} /* namespace eng */
