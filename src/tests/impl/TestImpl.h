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
uint32_t Generator::captures = 0;

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

template<class BOARD_T>
void Board::test()
{
	BOARD_T board;
	board.set_fen_pos("8/8/8/1P6/8/8/8/8 w KQkq - 0 0");
	assert(board.move("b5b6"));

	board.set_fen_pos("8/8/8/8/8/8/P7/8 w KQkq - 0 0");
	assert(board.move("a2a3"));
	board.take_back();
	assert(board.move("a2a4"));

	board.set_fen_pos("8/8/8/8/1p6/8/8/8 b KQkq - 0 0");
	assert(board.move("b4b3"));

	board.set_fen_pos("8/p7/8/8/8/8/8/8 b KQkq - 0 0");
	assert(board.move("a7a6"));
	board.take_back();
	assert(board.move("a7a5"));

	board.set_fen_pos("8/8/1p6/3N4/8/8/8/8 w KQkq - 0 0");
	assert(board.move("d5b6"));
	board.take_back();
	assert(board.move("d5b4"));
	board.take_back();
	assert(board.move("d5c7"));
	board.take_back();
	assert(board.move("d5c3"));
	board.take_back();
	assert(board.move("d5e3"));
	board.take_back();
	assert(board.move("d5e7"));
	board.take_back();
	assert(board.move("d5f6"));
	board.take_back();
	assert(board.move("d5f4"));

	board.set_fen_pos("B7/1p6/8/8/8/8/8/8 w KQkq - 0 0");
	assert(board.move("a8b7"));

	board.set_fen_pos("8/8/8/8/8/3n4/1KP3r1/8 w - - 0 1");
	assert(!board.move("c2d3"));

	board.set_fen_pos("8/8/8/8/8/4n3/2KP4/7r w - - 0 1");
	assert(board.move("d2e3"));

	board.set_fen_pos("8/8/8/2b5/8/4n3/2KP4/7r w - - 0 1");
	assert(board.move("d2e3"));

	board.set_fen_pos("8/2q5/8/2b5/8/4n3/2KP4/7r w - - 0 1");
	assert(board.move("d2e3"));

	board.set_fen_pos("3q4/8/8/2pP4/8/8/8/3K4 w - c6 0 1");
	assert(!board.move("d5c6"));

	board.set_fen_pos("3q3k/8/8/2p5/3P4/8/8/B2K4 w - - 0 1");
	assert(board.move("d4d5"));
	board.take_back();
	assert(!board.move("d4c5"));

	board.set_fen_pos("8/8/8/pppppppp/PPPPPPPP/8/8/8 w - - 0 1");
	assert(board.move("a4b5"));
	board.take_back();
	assert(board.move("b4a5"));
	board.take_back();
	assert(board.move("b4c5"));
	board.take_back();
	assert(board.move("c4b5"));
	board.take_back();
	assert(board.move("c4d5"));
	board.take_back();
	assert(board.move("d4c5"));
	board.take_back();
	assert(board.move("d4e5"));
	board.take_back();
	assert(board.move("e4d5"));
	board.take_back();
	assert(board.move("e4f5"));
	board.take_back();
	assert(board.move("f4e5"));
	board.take_back();
	assert(board.move("f4g5"));
	board.take_back();
	assert(board.move("g4f5"));
	board.take_back();
	assert(board.move("g4h5"));
	board.take_back();
	assert(board.move("h4g5"));

}

template<class BOARD_T, class MOVGEN_T>
void Generator::test()
{
	BOARD_T board;
	MOVGEN_T gen;
	std::vector<typename BOARD_T::GenMove_t> moves;
	board.set_fen_pos("8/8/8/1P6/8/8/8/8 w KQkq - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 1);
	moves.clear();

	board.set_fen_pos("8/8/8/8/8/8/P7/8 w KQkq - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/8/1p6/8/8/8 b KQkq - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 1);
	moves.clear();

	board.set_fen_pos("8/p7/8/8/8/8/8/8 b KQkq - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/p7/1P6/8/8/8/8/8 b KQkq - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 3);
	moves.clear();

	board.set_fen_pos("8/8/p7/1P6/8/8/8/8 b KQkq - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 2);
	moves.clear();

	board.set_fen_pos("8/8/8/8/3N4/8/8/8 w KQkq - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 8);
	moves.clear();

	board.set_fen_pos("8/8/8/1p6/3N4/8/8/8 w KQkq - 0 0");
	gen.gen_moves(board, moves);
	assert(moves.size() == 8);
	moves.clear();

	board.set_fen_pos("8/8/8/8/8/3n4/1KP3r1/8 w - - 0 1");
	gen.gen_moves(board, moves);
	assert(moves.size() == 10);
	moves.clear();

	board.set_fen_pos("3q3k/8/8/2p5/3P4/8/8/B2K4 w - - 0 1");
	gen.gen_moves(board, moves);
	assert(moves.size() == 9);
	moves.clear();

	board.set_fen_pos("3q2k1/8/8/2pP4/8/8/8/B2K4 w - c6 0 1");
	gen.gen_moves(board, moves);
	assert(moves.size() == 14);
	moves.clear();

	board.set_fen_pos("8/8/8/pppppppp/PPPPPPPP/8/8/8 w - - 0 1");
	gen.gen_moves(board, moves);
	assert(moves.size() == 14);
	moves.clear();

}

template<class BOARD_T, class MOVGEN_T>
void Generator::perft(uint32_t depth)
{
	BOARD_T board;
	MOVGEN_T gen;
	//board.set_fen_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w KQkq - 0 0");
	Generator::checks = 0;
	Generator::mates = 0;
	Generator::captures = 0;
	std::vector<typename BOARD_T::GenMove_t> moves;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	g_log << "depth " << depth << ", started at " << ctime(&t) << std::endl;
	uint64_t nodes = _perft(depth, board, gen);
	std::chrono::steady_clock::duration duration = std::chrono::steady_clock::now() - start;
	double seconds = static_cast<double>(duration.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
	g_log << "nodes: " << nodes << std::endl;
	g_log << "captures: " << captures << std::endl;
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
		uint32_t pieces;
		if (depth == 1)
			pieces = count_pieces(board);

		if (board.move(*it))
		{
			if (depth == 1 && pieces != count_pieces(board))
				captures++;
			nodes += _perft(depth - 1, board, gen);
			board.take_back();
		}

	}
	return nodes;
}

template<class BOARD_T>
uint32_t Generator::count_pieces(BOARD_T& board)
{
	uint32_t ret = 0;
	for (int i = 0; i < 64; i++)
	{
		if (board.get_piece(i) != eng::BoardBase::EMPTY)
			ret++;
	}
	return ret;
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
