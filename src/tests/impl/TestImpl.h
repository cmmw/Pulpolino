/*
 * Test.cpp
 *
 *  Created on: 10.05.2014
 *      Author: Christian
 */

#include <cassert>
#include "../../engine/BoardBase.h"
namespace test
{

uint32_t Test::checks = 0;
uint32_t Test::mates = 0;

template<class BOARD_T>
void Test::ep()
{
	BOARD_T board;
	typename BOARD_T::Piece_t old, capt;

	board.move("e2e4");
	board.move("d7d6");
	board.move("e4e5");
	board.move("d6d5");
	assert(!board.move("e5d6"));

	board.take_back();
	board.take_back();
	board.take_back();

	board.move("a7a6");
	board.move("e4e5");
	board.move("d7d5");
	old = board.get_piece(36);
	capt = board.get_piece(35);
	assert(board.move("e5d6"));

	assert(board.get_piece(35) == eng::BoardBase::EMPTY);
	assert(board.get_piece(36) == eng::BoardBase::EMPTY);
	assert(board.get_piece(43) == old);

	board.take_back();
	assert(board.get_piece(35) == capt);
	assert(board.get_piece(36) == old);
	assert(board.get_piece(43) == eng::BoardBase::EMPTY);

	board.take_back();
	assert(board.get_piece(35) == eng::BoardBase::EMPTY);
	assert(board.get_piece(51) == capt);

	board.take_back();
	assert(board.get_piece(28) == old);

	board.reset();

	board.move("a2a3");
	board.move("d7d5");
	board.move("e2e3");
	board.move("d5d4");
	board.move("e3e4");
	assert(!board.move("d4e3"));

	board.take_back();
	board.take_back();
	board.take_back();

	board.move("a3a4");
	board.move("d5d4");
	board.move("e2e4");
	old = board.get_piece(27);
	capt = board.get_piece(28);
	assert(board.move("d4e3"));
	assert(board.get_piece(28) == eng::BoardBase::EMPTY);
	assert(board.get_piece(27) == eng::BoardBase::EMPTY);
	assert(board.get_piece(20) == old);

	board.take_back();
	assert(board.get_piece(28) == capt);
	assert(board.get_piece(27) == old);
	assert(board.get_piece(20) == eng::BoardBase::EMPTY);

	board.take_back();
	assert(board.get_piece(28) == eng::BoardBase::EMPTY);
	assert(board.get_piece(12) == capt);

	board.take_back();
	assert(board.get_piece(35) == old);

	g_log << "En passant test ok" << std::endl;
}

template<class BOARD_T, class MOVGEN_T>
void Test::perft(uint32_t depth)
{
	BOARD_T board;
	MOVGEN_T gen;
	Test::checks = 0;
	Test::mates = 0;
	std::vector<typename BOARD_T::GenMove_t> moves;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	g_log << "depth " << depth << ", started at " << ctime(&t) << std::endl;
	uint64_t nodes = _perft(depth, board, gen);
	std::chrono::steady_clock::duration duration = std::chrono::steady_clock::now() - start;
	double seconds = static_cast<double>(duration.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
	g_log << "nodes: " << nodes << std::endl;
	g_log << "checks: " << Test::checks << std::endl;
	g_log << "mates: " << Test::mates << std::endl;
	g_log << "time needed: " << seconds << " seconds, " << (seconds / 60) << " minutes" << std::endl << std::endl;
}

template<class BOARD_T, class MOVGEN_T>
uint64_t Test::_perft(uint32_t depth, BOARD_T& board, MOVGEN_T& gen)
{
	uint64_t nodes = 0;

	std::vector<typename BOARD_T::GenMove_t> moves;

	if (depth == 0)
	{
		if (board.in_check(board.get_color()))
		{
			bool no_move = true;
			checks++;
			gen.gen_moves(board, moves);
			for (auto it = moves.begin(); it != moves.end(); it++)
			{
				if (!board.move(*it))
				{
					continue;
				}
				no_move = false;
				board.take_back();
				break;
			}
			if (no_move)
			{
				mates++;
			}
		}
		return 1;
	}

	gen.gen_moves(board, moves);
	for (auto it = moves.begin(); it != moves.end(); it++)
	{
		if (board.move(*it))
		{
			nodes += _perft(depth - 1, board, gen);
			board.take_back();
		}
	}
	return nodes;
}
} /* namespace eng */
