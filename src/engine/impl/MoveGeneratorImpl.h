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

	/* TODO missing moves: castle, promoting*/
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
				int32_t step, row, file;
				row = i >> 3;
				file = i % 8;
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

				/*en passant*/
				if (file != 0)
				{
					if (row == 4 && board.get_piece(i - 1) == BoardBase::PAWN && board.get_color(i - 1) == BOARD_T::BLACK && c1 == BOARD_T::WHITE)
					{
						if (board.get_piece(i + 8 - 1) == BoardBase::EMPTY)
							moves.push_back(board.gen_mov(i, i + 8 - 1));
					}

					if (row == 3 && board.get_piece(i - 1) == BoardBase::PAWN && board.get_color(i - 1) == BOARD_T::WHITE && c1 == BOARD_T::BLACK)
					{
						if (board.get_piece(i - 8 - 1) == BoardBase::EMPTY)
							moves.push_back(board.gen_mov(i, i - 8 - 1));
					}
				}

				if (file != 7)
				{
					if (row == 4 && board.get_piece(i + 1) == BoardBase::PAWN && board.get_color(i + 1) == BOARD_T::BLACK && c1 == BOARD_T::WHITE)
					{
						if (board.get_piece(i + 8 + 1) == BoardBase::EMPTY)
							moves.push_back(board.gen_mov(i, i + 8 + 1));
					}

					if (row == 3 && board.get_piece(i + 1) == BoardBase::PAWN && board.get_color(i + 1) == BOARD_T::WHITE && c1 == BOARD_T::BLACK)
					{
						if (board.get_piece(i - 8 + 1) == BoardBase::EMPTY)
							moves.push_back(board.gen_mov(i, i - 8 + 1));
					}
				}

			}
		}
	}
}

} /* namespace eng */
