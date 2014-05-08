/*
 * Evaluator.cpp
 *
 *  Created on: 07.05.2014
 *      Author: Christian
 */

#include "../Evaluator.h"
#include "../BoardBase.h"
#include "../../Globals.h"

namespace eng
{

template<class BOARD_T>
int Evaluator<BOARD_T>::sq_values[64] =
		{
				1, 1, 1, 1, 1, 1, 1, 1,
				2, 2, 2, 2, 2, 2, 2, 3,
				3, 3, 3, 3, 3, 3, 3, 3,
				4, 4, 4, 5, 5, 4, 4, 4,
				4, 4, 4, 5, 5, 4, 4, 4,
				3, 3, 3, 3, 3, 3, 3, 3,
				2, 2, 2, 2, 2, 2, 2, 2,
				1, 1, 1, 1, 1, 1, 1, 1,
		};

template<class BOARD_T>
int Evaluator<BOARD_T>::pc_values[6] =
		{ 1, 3, 3, 5, 9, 100 };

template<class BOARD_T>
int32_t Evaluator<BOARD_T>::operator()(BOARD_T& board)
{
	int32_t val = 0;

	for (int i = 0; i < 63; i++)
	{
		typename BOARD_T::Piece_t piece = board.get_piece(i);
		typename BOARD_T::Color_t color = board.get_color(i);

//		Material
		if (piece != BoardBase::EMPTY)
		{
			if (color == BOARD_T::WHITE)
			{
				val += pc_values[piece] * 3;
			}
			else
			{
				val -= pc_values[piece] * 3;
			}
		}

//		Attacks
		if (piece != BoardBase::EMPTY)
		{
			if (color == BOARD_T::WHITE)
			{
				if (board.is_attacked(BOARD_T::WHITE, i))
				{
					val -= pc_values[piece] * 2;
				}
			}
			else
			{
				if (board.is_attacked(BOARD_T::BLACK, i))
				{
					val += pc_values[piece] * 2;
				}
			}
		}

//		Position
		if (piece != BoardBase::EMPTY)
		{

			if (color == BOARD_T::WHITE)
			{
				if (piece != BoardBase::KING)
				{
					val += sq_values[i];
				}
			}
			else
			{
				if (piece != BoardBase::KING)
				{
					val -= sq_values[i];
				}
			}
		}

	}
	return val;
}

} /* namespace eng */
