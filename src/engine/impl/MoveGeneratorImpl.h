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

	/*castle white*/
	if (c1 == BOARD_T::WHITE && board.get_piece(4) == BoardBase::KING && board.get_color(4) == BOARD_T::WHITE)
	{
		bool castle = true;

		if (board.get_piece(0) == BoardBase::ROOK && board.get_color(0) == BOARD_T::WHITE)
		{
			for (int i = 1; i < 4; i++)
			{
				if (board.get_piece(i) != BoardBase::EMPTY)
				{
					castle = false;
					break;
				}
			}

			if (castle)
			{
				moves.push_back(board.gen_mov(4, 2, SCR_CAST));
			}
		}

		castle = true;
		if (board.get_piece(7) == BoardBase::ROOK
				&& board.get_color(7) == BOARD_T::WHITE)
		{
			for (int i = 5; i < 7; i++)
			{
				if (board.get_piece(i) != BoardBase::EMPTY)
				{
					castle = false;
					break;
				}
			}

			if (castle)
			{
				moves.push_back(board.gen_mov(4, 6, SCR_CAST));
			}
		}
	}

	/*castle black*/
	if (c1 == BOARD_T::BLACK && board.get_piece(60) == BoardBase::KING && board.get_color(60) == BOARD_T::BLACK)
	{
		bool castle = true;
		if (board.get_piece(56) == BoardBase::ROOK && board.get_color(56) == BOARD_T::BLACK)
		{
			for (int i = 57; i < 60; i++)
			{
				if (board.get_piece(i) != BoardBase::EMPTY)
				{
					castle = false;
					break;
				}
			}

			if (castle)
			{
				moves.push_back(board.gen_mov(60, 58, SCR_CAST));
			}
		}

		castle = true;
		if (board.get_piece(63) == BoardBase::ROOK && board.get_color(63) == BOARD_T::BLACK)
		{
			for (int i = 61; i < 63; i++)
			{
				if (board.get_piece(i) != BoardBase::EMPTY)
				{
					castle = false;
					break;
				}
			}

			if (castle)
			{
				moves.push_back(board.gen_mov(60, 62, SCR_CAST));
			}
		}

	}

	for (int32_t i = 0; i < 64; i++)
	{
		piece = board.get_piece(i);
		if (board.get_color(i) == c1 && piece != BoardBase::EMPTY)
		{
			if (piece != BoardBase::PAWN)
			{
				for (int32_t j = 0; j < BoardBase::_offsets[piece]; j++)
				{
					for (int32_t n = i;;)
					{
						n = BoardBase::_mailbox[BoardBase::_mailbox64[n] + BoardBase::_offset[piece][j]];

						if (n == -1)
							break;

						if (board.get_piece(n) != BoardBase::EMPTY)
						{
							if (board.get_color(n) == c2)
							{
								moves.push_back(board.gen_mov(i, n, SCR_CAPT));
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
					if ((c1 == BOARD_T::WHITE && row == 6) || (c1 == BOARD_T::BLACK && row == 1))
					{
						for (int j = 1; j < 5; j++)
							moves.push_back(board.gen_mov(i, i + (step * 8), SCR_PRO, static_cast<BoardBase::Piece_t>(j)));
					}
					else
					{
						moves.push_back(board.gen_mov(i, i + (step * 8)));
					}
					if (((c1 == BOARD_T::WHITE && row == 1) || (c1 == BOARD_T::BLACK && row == 6)) && board.get_piece(i + (2 * step * 8)) == BoardBase::EMPTY)
					{
						moves.push_back(board.gen_mov(i, i + (2 * step * 8)));
					}
				}

				/*pawn capture*/
				int8_t n;
				n = BoardBase::_mailbox[BoardBase::_mailbox64[i] + step + (step * 10)]; /*10 = offset in mailbox for pawns*/
				if (n != -1 && board.get_piece(n) != BoardBase::EMPTY)
				{
					if (board.get_color(n) == c2)
					{
						if ((c1 == BOARD_T::WHITE && row == 6) || (c1 == BOARD_T::BLACK && row == 1))
						{
							for (uint8_t j = 1; j < 5; j++)
								moves.push_back(board.gen_mov(i, n, SCR_PRO_CAPT, static_cast<BoardBase::Piece_t>(j)));
						}
						else
						{
							moves.push_back(board.gen_mov(i, n, SCR_PAW_CAPT));
						}
					}
				}

				n = BoardBase::_mailbox[BoardBase::_mailbox64[i] - step + (step * 10)];
				if (n != -1 && board.get_piece(n) != BoardBase::EMPTY)
				{
					if (board.get_color(n) == c2)
					{
						if ((c1 == BOARD_T::WHITE && row == 6) || (c1 == BOARD_T::BLACK && row == 1))
						{
							for (int j = 1; j < 5; j++)
								moves.push_back(board.gen_mov(i, n, SCR_PRO_CAPT, static_cast<BoardBase::Piece_t>(j)));
						}
						else
						{
							moves.push_back(board.gen_mov(i, n, SCR_PAW_CAPT));
						}
					}
				}

				/*en passant*/
				if (file != 0)
				{
					if (row == 4 && board.get_piece(i - 1) == BoardBase::PAWN && board.get_color(i - 1) == BOARD_T::BLACK && c1 == BOARD_T::WHITE)
					{
						if (board.get_piece(i + 8 - 1) == BoardBase::EMPTY)
							moves.push_back(board.gen_mov(i, i + 8 - 1, SCR_EP_CAPT));
					}

					if (row == 3 && board.get_piece(i - 1) == BoardBase::PAWN && board.get_color(i - 1) == BOARD_T::WHITE && c1 == BOARD_T::BLACK)
					{
						if (board.get_piece(i - 8 - 1) == BoardBase::EMPTY)
							moves.push_back(board.gen_mov(i, i - 8 - 1, SCR_EP_CAPT));
					}
				}

				if (file != 7)
				{
					if (row == 4 && board.get_piece(i + 1) == BoardBase::PAWN && board.get_color(i + 1) == BOARD_T::BLACK && c1 == BOARD_T::WHITE)
					{
						if (board.get_piece(i + 8 + 1) == BoardBase::EMPTY)
							moves.push_back(board.gen_mov(i, i + 8 + 1, SCR_EP_CAPT));
					}

					if (row == 3 && board.get_piece(i + 1) == BoardBase::PAWN && board.get_color(i + 1) == BOARD_T::WHITE && c1 == BOARD_T::BLACK)
					{
						if (board.get_piece(i - 8 + 1) == BoardBase::EMPTY)
							moves.push_back(board.gen_mov(i, i - 8 + 1, SCR_EP_CAPT));
					}
				}

			}
		}
	}
}

} /* namespace eng */
