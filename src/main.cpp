//============================================================================
// Name        : ChessEngine.cpp
// Author      : Christian Wagner
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include <algorithm>
#include <iterator>
#include <sstream>

#include "engine/Board.h"
#include "engine/Evaluator.h"

#include "main.h"
#include "Globals.h"

Logger g_log;
eng::Engine<eng::Board, eng::MoveGenerator<eng::Board>, eng::Evaluator<eng::Board>> engine;

/*
 * MoveGenerator tester, compare results with:
 *
 * http://chessprogramming.wikispaces.com/Perft+Results
 */

static int checks = 0;
static int mates = 0;

template<class BOARD_T, class MOVGEN_T>
uint64_t _perft(uint32_t depth, BOARD_T& board, MOVGEN_T& gen)
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

void perft(uint32_t depth)
{
	eng::Board board;
	eng::MoveGenerator<eng::Board> gen;
	std::vector<eng::Board::GenMove_t> moves;
	board.print_board();
	uint64_t nodes = _perft(depth, board, gen);
	g_log << "nodes: " << nodes << std::endl;
	g_log << "checks: " << checks << std::endl;
	g_log << "mates: " << mates << std::endl;
}

int main()
{
	perft(5);
//	std::thread th(uci_input_th);
//	engine.start();
//	th.join();
	return 0;
}

void uci_input_th()
{
	/*todo: finish implementation of uci commands*/
	bool quit;
	while (!quit)
	{
		std::string cmd;
		std::getline(std::cin, cmd);

		g_log << "info string proceeding: " << cmd << std::endl;

		if (!cmd.compare("uci"))
		{
			g_log << "id name Pulpolino" << std::endl;
			g_log << "id author Christian Wagner" << std::endl;
			g_log << "uciok" << std::endl;
		}
		else if (!cmd.compare("debug"))
		{

		}
		else if (!cmd.compare("isready"))
		{
			g_log << "readyok" << std::endl;
		}
		else if (!cmd.compare("setoption"))
		{

		}
		else if (!cmd.compare("register"))
		{

		}
		else if (!cmd.compare("ucinewgame"))
		{

		}
		else if (!cmd.compare(0, 8, "position"))
		{
			engine.position(cmd);
		}
		else if (!cmd.compare(0, 2, "go"))
		{
			engine.go();
		}
		else if (!cmd.compare("stop"))
		{
			engine.stop();
		}
		else if (!cmd.compare("ponderhit"))
		{

		}
		else if (!cmd.compare("quit"))
		{
			quit = true;
			engine.quit();
		}
		else if (!cmd.compare("print"))
		{
			engine.print_board();
		}
		else
		{
			g_log << "info string unknown cmd" << std::endl;
		}

	}
}
