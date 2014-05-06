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

#include "main.h"
#include "Globals.h"

Logger g_log;
eng::Engine<eng::Board, eng::MoveGenerator<eng::Board>> engine;

/*
 * http://chessprogramming.wikispaces.com/Perft+Results
 */

void test_gen()
{
	eng::Board board;
	eng::MoveGenerator<eng::Board> gen;
	std::vector<eng::Board::GenMove_t> moves;
	gen.generate_moves(board, moves, eng::Board::WHITE);
	g_log << "moves calculated: " << moves.size() << std::endl;
}

int main()
{
	test_gen();

//	std::thread th(uci_input_th);
//	engine.start();
//	th.join();
	return 0;
}

void uci_input_th()
{
	bool quit;
	while (!quit)
	{
		std::string cmd;
		std::getline(std::cin, cmd);

		g_log << "info string proceeding: " << cmd << std::endl;

		if (!cmd.compare("uci"))
		{
			g_log << "id name ChessEngine" << std::endl;
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
		else if (!cmd.compare("go"))
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
		else
		{
			g_log << "info unknown cmd" << std::endl;
		}

	}
}
