/*
 * Evaluator.cpp
 *
 *  Created on: 07.05.2014
 *      Author: Christian
 */

#include "Evaluator.h"

namespace eng
{

template<class BOARD_T>
Evaluator<BOARD_T>::Evaluator()
{

}

template<class BOARD_T>
int32_t Evaluator<BOARD_T>::operator()(BOARD_T& board)
{
	return std::rand() % 200 + 1 - 100;
}

} /* namespace eng */
