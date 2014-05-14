/*
 * Evaluator.cpp
 *
 *  Created on: 07.05.2014
 *      Author: Christian
 */

#include "../Evaluator.h"
#include "../BoardBase.h"
#include "../../globals.h"

namespace eng
{

template<class BOARD_T>
int Evaluator<BOARD_T>::pc_values[6] =
		{ 100, 320, 330, 500, 900, 20000 };

template<class BOARD_T>
int Evaluator<BOARD_T>::sq_pawns[64] =
		{
				0, 0, 0, 0, 0, 0, 0, 0,
				50, 50, 50, 50, 50, 50, 50, 50,
				10, 10, 20, 30, 30, 20, 10, 10,
				5, 5, 10, 25, 25, 10, 5, 5,
				0, 0, 0, 20, 20, 0, 0, 0,
				5, -5, -10, 0, 0, -10, -5, 5,
				5, 10, 10, -20, -20, 10, 10, 5,
				0, 0, 0, 0, 0, 0, 0, 0
		};

template<class BOARD_T>
int Evaluator<BOARD_T>::sq_knights[64] =
		{
				-50, -40, -30, -30, -30, -30, -40, -50,
				-40, -20, 0, 0, 0, 0, -20, -40,
				-30, 0, 10, 15, 15, 10, 0, -30,
				-30, 5, 15, 20, 20, 15, 5, -30,
				-30, 0, 15, 20, 20, 15, 0, -30,
				-30, 5, 10, 15, 15, 10, 5, -30,
				-40, -20, 0, 5, 5, 0, -20, -40,
				-50, -40, -30, -30, -30, -30, -40, -50,
		};

template<class BOARD_T>
int Evaluator<BOARD_T>::sq_bishops[64] =
		{
				-20, -10, -10, -10, -10, -10, -10, -20,
				-10, 0, 0, 0, 0, 0, 0, -10,
				-10, 0, 5, 10, 10, 5, 0, -10,
				-10, 5, 5, 10, 10, 5, 5, -10,
				-10, 0, 10, 10, 10, 10, 0, -10,
				-10, 10, 10, 10, 10, 10, 10, -10,
				-10, 5, 0, 0, 0, 0, 5, -10,
				-20, -10, -10, -10, -10, -10, -10, -20,
		};

template<class BOARD_T>
int Evaluator<BOARD_T>::sq_rooks[64] =
		{
				0, 0, 0, 0, 0, 0, 0, 0,
				5, 10, 10, 10, 10, 10, 10, 5,
				-5, 0, 0, 0, 0, 0, 0, -5,
				-5, 0, 0, 0, 0, 0, 0, -5,
				-5, 0, 0, 0, 0, 0, 0, -5,
				-5, 0, 0, 0, 0, 0, 0, -5,
				-5, 0, 0, 0, 0, 0, 0, -5,
				0, 0, 0, 5, 5, 0, 0, 0
		};

template<class BOARD_T>
int Evaluator<BOARD_T>::sq_queen[64] =
		{
				-20, -10, -10, -5, -5, -10, -10, -20,
				-10, 0, 0, 0, 0, 0, 0, -10,
				-10, 0, 5, 5, 5, 5, 0, -10,
				-5, 0, 5, 5, 5, 5, 0, -5,
				0, 0, 5, 5, 5, 5, 0, -5,
				-10, 5, 5, 5, 5, 5, 0, -10,
				-10, 0, 5, 0, 0, 0, 0, -10,
				-20, -10, -10, -5, -5, -10, -10, -20
		};

template<class BOARD_T>
int Evaluator<BOARD_T>::sq_king[64] =
		{
				-30, -40, -40, -50, -50, -40, -40, -30,
				-30, -40, -40, -50, -50, -40, -40, -30,
				-30, -40, -40, -50, -50, -40, -40, -30,
				-30, -40, -40, -50, -50, -40, -40, -30,
				-20, -30, -30, -40, -40, -30, -30, -20,
				-10, -20, -20, -20, -20, -20, -20, -10,
				20, 20, 0, 0, 0, 0, 20, 20,
				20, 30, 10, 0, 0, 10, 30, 20
		};

template<class BOARD_T>
int32_t Evaluator<BOARD_T>::operator()(BOARD_T& board)
{
	int32_t val = 0;
	int32_t idx, one;

	for (int i = 0; i < 64; i++)
	{
		typename BOARD_T::Piece_t piece = board.get_piece(i);
		typename BOARD_T::Color_t color = board.get_color(i);

		if (piece != BoardBase::EMPTY)
		{

			//Material
			if (color == BOARD_T::WHITE)
			{
				idx = 63 - i;
				val += pc_values[piece];
				one = 1;
			}
			else
			{
				idx = i;
				val -= pc_values[piece];
				one = -1;
			}

			//Position
			switch (piece)
			{
			case BoardBase::PAWN:
				val = val + one * Evaluator::sq_pawns[idx];
				break;
			case BoardBase::KNIGHT:
				val = val + one * Evaluator::sq_knights[idx];
				break;
			case BoardBase::BISHOP:
				val = val + one * Evaluator::sq_bishops[idx];
				break;
			case BoardBase::ROOK:
				val = val + one * Evaluator::sq_rooks[idx];
				break;
			case BoardBase::QUEEN:
				val = val + one * Evaluator::sq_queen[idx];
				break;
			case BoardBase::KING:
				val = val + one * Evaluator::sq_rooks[idx];
				break;

			default:
				break;
			}

		}

	}

	if (board.get_color() == BOARD_T::BLACK)
		val = -val;

//	board.print();
//	g_log << "eval: " << val << std::endl;

	return val;
}

} /* namespace eng */
