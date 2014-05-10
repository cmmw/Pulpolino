/*
 * tests.h
 *
 *  Created on: 10.05.2014
 *      Author: Christian
 */

#ifndef TESTS_H_
#define TESTS_H_

#include <chrono>
#include <ctime>
#include <vector>
#include <stdint.h>

#include "../globals.h"

namespace test
{

/*
 * http://chessprogramming.wikispaces.com/Perft+Results
 */

class Test
{

public:

	/*Move generator tester*/
	template<class BOARD_T, class MOVGEN_T>
	static void perft(uint32_t depth);

	/*Test en passant moves*/
	template<class BOARD_T>
	static void ep();

private:

	static uint32_t checks;
	static uint32_t mates;

	template<class BOARD_T, class MOVGEN_T>
	static uint64_t _perft(uint32_t depth, BOARD_T& board, MOVGEN_T& gen);

};

} /* namespace eng */

#include "impl/testImpl.h"

#endif /* TESTS_H_ */
