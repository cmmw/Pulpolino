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

uint32_t Generator::checks = 0;
uint32_t Generator::mates = 0;

template<class BOARD_T>
void Board::ep()
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

	board.set_fen_pos("8/8/8/3pP3/8/8/8/8 w - d6 0 0");
	assert(board.move("e5d6"));

	board.set_fen_pos("8/8/8/pP6/8/8/8/8 w - a6 0 0");
	assert(board.move("b5a6"));

	board.set_fen_pos("8/8/8/Pp6/8/8/8/8 w - b6 0 0");
	assert(board.move("a5b6"));

	board.set_fen_pos("8/8/8/6pP/8/8/8/8 w - g6 0 0");
	assert(board.move("h5g6"));

	board.set_fen_pos("8/8/8/6Pp/8/8/8/8 w - h6 0 0");
	assert(board.move("g5h6"));

	board.set_fen_pos("8/1p6/8/P7/8/8/8/8 b - - 0 0");
	assert(board.move("b7b5"));
	assert(board.move("a5b6"));
	assert(board.get_piece(33) == eng::BoardBase::EMPTY);

	board.set_fen_pos("8/p7/8/1P6/8/8/8/8 b - - 0 0");
	assert(board.move("a7a5"));
	assert(board.move("b5a6"));
	assert(board.get_piece(32) == eng::BoardBase::EMPTY);

	board.set_fen_pos("8/7p/8/6P1/8/8/8/8 b - - 0 0");
	assert(board.move("h7h5"));
	assert(board.move("g5h6"));
	assert(board.get_piece(39) == eng::BoardBase::EMPTY);

	board.set_fen_pos("8/6p1/8/7P/8/8/8/8 b - - 0 0");
	assert(board.move("g7g5"));
	assert(board.move("h5g6"));
	assert(board.get_piece(38) == eng::BoardBase::EMPTY);

	board.set_fen_pos("8/8/8/8/3pP3/8/8/8 b - e3 0 0");
	assert(board.move("d4e3"));

	board.set_fen_pos("8/8/8/8/pP6/8/8/8 b - b3 0 0");
	assert(board.move("a4b3"));

	board.set_fen_pos("8/8/8/8/Pp6/8/8/8 b - a3 0 0");
	assert(board.move("b4a3"));

	board.set_fen_pos("8/8/8/8/6pP/8/8/8 b - h3 0 0");
	assert(board.move("g4h3"));

	board.set_fen_pos("8/8/8/8/6Pp/8/8/8 b - g3 0 0");
	assert(board.move("h4g3"));

	board.set_fen_pos("8/8/8/8/1p6/8/P7/8 w - - 0 0");
	assert(board.move("a2a4"));
	assert(board.move("b4a3"));
	assert(board.get_piece(24) == eng::BoardBase::EMPTY);

	board.set_fen_pos("8/8/8/8/p7/8/1P6/8 w - - 0 0");
	assert(board.move("b2b4"));
	assert(board.move("a4b3"));
	assert(board.get_piece(25) == eng::BoardBase::EMPTY);

	board.set_fen_pos("8/8/8/8/6p1/8/7P/8 w - - 0 0");
	assert(board.move("h2h4"));
	assert(board.move("g4h3"));
	assert(board.get_piece(31) == eng::BoardBase::EMPTY);

	board.set_fen_pos("8/8/8/8/7p/8/6P1/8 w - - 0 0");
	assert(board.move("g2g4"));
	assert(board.move("h4g3"));
	assert(board.get_piece(30) == eng::BoardBase::EMPTY);

	g_log << "Board en passant test ok" << std::endl;
}

template<class BOARD_T, class MOVGEN_T>
void Generator::perft(uint32_t depth)
{
	BOARD_T board;
	MOVGEN_T gen;
	Generator::checks = 0;
	Generator::mates = 0;
	std::vector<typename BOARD_T::GenMove_t> moves;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	g_log << "depth " << depth << ", started at " << ctime(&t) << std::endl;
	uint64_t nodes = _perft(depth, board, gen);
	std::chrono::steady_clock::duration duration = std::chrono::steady_clock::now() - start;
	double seconds = static_cast<double>(duration.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
	g_log << "nodes: " << nodes << std::endl;
	g_log << "checks: " << Generator::checks << std::endl;
	g_log << "mates: " << Generator::mates << std::endl;
	g_log << "time needed: " << seconds << " seconds, " << (seconds / 60) << " minutes" << std::endl << std::endl;
}

template<class BOARD_T, class MOVGEN_T>
uint64_t Generator::_perft(uint32_t depth, BOARD_T& board, MOVGEN_T& gen)
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

template<class BOARD_T, class MOVGEN_T>
void Generator::ep()
{
	BOARD_T board;
	MOVGEN_T gen;
	std::vector<typename BOARD_T::GenMove_t> moves;
	board.set_fen_pos("8/8/8/pP6/8/8/8/8 w - - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/Pp6/8/8/8/8 w - - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/8/Pp6/8/8/8 b - - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/8/pP6/8/8/8 b - - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/6pP/8/8/8/8 w - - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/6Pp/8/8/8/8 w - - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/3pP3/8/8/8/8 w - - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/3p4/8/4P3/8/8/8/8 b - - 0 0");
	gen.gen_moves(board, moves);
	assert(board.move("d7d5"));
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/p7/8/1P6/8/8/8/8 b - - 0 0");
	gen.gen_moves(board, moves);
	assert(board.move("a7a5"));
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/7p/8/6P1/8/8/8/8 b - - 0 0");
	gen.gen_moves(board, moves);
	assert(board.move("h7h5"));
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/1p6/8/P7/8/8/8/8 b - - 0 0");
	gen.gen_moves(board, moves);
	assert(board.move("b7b5"));
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/6p1/8/7P/8/8/8/8 b - - 0 0");
	gen.gen_moves(board, moves);
	assert(board.move("g7g5"));
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/8/pP6/8/8/8 b - - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/8/6pP/8/8/8 b - - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/8/3pP3/8/8/8 b - - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/8/3p4/8/4P3/8 w - - 0 0");
	assert(board.move("e2e4"));
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/8/7p/8/6P1/8 w - - 0 0");
	assert(board.move("g2g4"));
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/8/p7/8/1P6/8 w - - 0 0");
	assert(board.move("b2b4"));
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/8/1p6/8/P7/8 w - - 0 0");
	assert(board.move("a2a4"));
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/8/6p1/8/7P/8 w - - 0 0");
	assert(board.move("h2h4"));
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	g_log << "Generator en passant test ok" << std::endl;

}

} /* namespace eng */
