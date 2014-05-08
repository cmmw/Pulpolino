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
	Evaluator() = default;
	~Evaluator() = default;

	Evaluator(const Evaluator&) = delete;
	Evaluator(Evaluator&&) = delete;
	Evaluator& operator=(const Evaluator&) = delete;

	int32_t operator()(BOARD_T& board);


private:
	static int sq_values[64];
	static int pc_values[6];
};

} /* namespace eng */

#include "impl/EvaluatorImpl.h"

#endif /* EVALUATOR_H_ */
