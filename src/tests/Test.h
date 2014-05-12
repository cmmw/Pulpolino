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

	template<class BOARD_T>
	static void checks();

	template<class BOARD_T>
	static void castle();

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

	template<class BOARD_T, class MOVGEN_T>
	static void checks();

	template<class BOARD_T, class MOVGEN_T>
	static void calc();

private:
	static uint32_t _checks;
	static uint32_t _mates;
	static uint32_t _captures;

	template<class BOARD_T, class MOVGEN_T>
	static uint64_t _perft(uint32_t depth, BOARD_T& board, MOVGEN_T& gen);

	template<class BOARD_T>
	static uint32_t count_pieces(BOARD_T& board);

	template<class BOARD_T, class MOVGEN_T>
	static bool is_mate(BOARD_T& t, MOVGEN_T& gen);

};

} /* namespace eng */

#include "impl/testImpl.h"

#endif /* TESTS_H_ */
