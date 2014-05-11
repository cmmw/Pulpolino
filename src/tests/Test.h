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

class Board
{

public:

	/*Test en passant moves*/
	template<class BOARD_T>
	static void ep();

	template<class BOARD_T>
	static void test();

};

class Generator
{

public:
	template<class BOARD_T, class MOVGEN_T>
	static void perft(uint32_t depth);

	template<class BOARD_T, class MOVGEN_T>
	static void ep();

	template<class BOARD_T, class MOVGEN_T>
	static void test();

private:
	static uint32_t checks;
	static uint32_t mates;
	static uint32_t captures;

	template<class BOARD_T, class MOVGEN_T>
	static uint64_t _perft(uint32_t depth, BOARD_T& board, MOVGEN_T& gen);

	template<class BOARD_T>
	static uint32_t count_pieces(BOARD_T& board);

};

} /* namespace eng */

#include "impl/testImpl.h"

#endif /* TESTS_H_ */
