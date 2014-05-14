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
	static int pc_values[6];
	static int sq_pawns[64];
	static int sq_knights[64];
	static int sq_bishops[64];
	static int sq_rooks[64];
	static int sq_queen[64];
	static int sq_king[64];
};

} /* namespace eng */

#include "impl/EvaluatorImpl.h"

#endif /* EVALUATOR_H_ */
