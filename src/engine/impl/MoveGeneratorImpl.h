/*
 * MoveGenerator.cpp
 *
 *  Created on: 05.05.2014
 *      Author: Christian
 */

#include "../MoveGenerator.h"
#include "../../globals.h"
#include "../BoardBase.h"
#include "../Board.h"

namespace eng
{

template<class BOARD_T>
void MoveGenerator<BOARD_T>::gen_moves(BOARD_T& board, std::vector<typename BOARD_T::GenMove_t>& moves)
{
	typename BOARD_T::Piece_t piece;
	typename BOARD_T::Color_t c1;
	typename BOARD_T::Color_t c2;

	c1 = board.get_color();
	c2 = (c1 == BOARD_T::WHITE) ? BOARD_T::BLACK : BOARD_T::WHITE;

	/* TODO missing moves: castle, promoting, en passant */
	for (int i = 0; i < 64; i++)
	{
		piece = board.get_piece(i);
		if (board.get_color(i) == c1 && piece != BoardBase::EMPTY)
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
				/*Single and double push*/
				int32_t step, row;
				row = i >> 3;
				step = (c1 == BOARD_T::WHITE) ? 1 : -1;
				if (board.get_piece(i + (step * 8)) == BoardBase::EMPTY)
				{
					moves.push_back(board.gen_mov(i, i + (step * 8)));
					if (((c1 == BOARD_T::WHITE && row == 1) || (c1 == BOARD_T::BLACK && row == 6)) && board.get_piece(i + (2 * step * 8)) == BoardBase::EMPTY)
					{
						moves.push_back(board.gen_mov(i, i + (2 * step * 8)));
					}
				}

				/*pawn capture*/
				int32_t n;
				n = BoardBase::_mailbox[BoardBase::_mailbox64[i] + step + (step * 10)]; /*10 = offset in mailbox for pawns*/
				if (n != -1 && board.get_piece(n) != BoardBase::EMPTY)
				{
					if (board.get_color(n) == c2)
					{
						moves.push_back(board.gen_mov(i, n));
					}
				}

				n = BoardBase::_mailbox[BoardBase::_mailbox64[i] - step + (step * 10)];
				if (n != -1 && board.get_piece(n) != BoardBase::EMPTY)
				{
					if (board.get_color(n) == c2)
					{
						moves.push_back(board.gen_mov(i, n));
					}
				}
			}
		}
	}
}

} /* namespace eng */
