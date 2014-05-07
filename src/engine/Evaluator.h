/*
 * Evaluator.h
 *
 *  Created on: 07.05.2014
 *      Author: Christian
 */

#ifndef EVALUATOR_H_
#define EVALUATOR_H_

namespace eng
{

template<class BOARD_T>
class Evaluator
{
public:
	Evaluator();
	~Evaluator() = default;

	Evaluator(const Evaluator&) = delete;
	Evaluator(Evaluator&&) = delete;
	Evaluator& operator=(const Evaluator&) = delete;

	int32_t operator()(BOARD_T& board);
};

} /* namespace eng */

#include "Evaluator.cpp"

#endif /* EVALUATOR_H_ */
