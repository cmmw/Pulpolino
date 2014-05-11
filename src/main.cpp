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
#include "tests/Test.h"
#include "globals.h"

Logger g_log;
static eng::Engine<eng::Board, eng::MoveGenerator<eng::Board>, eng::Evaluator<eng::Board>> engine;

void run_tests()
{
	test::Generator::perft<eng::Board, eng::MoveGenerator<eng::Board>>(5);
	test::Board::test<eng::Board>();
	test::Board::ep<eng::Board>();
	test::Board::checks<eng::Board>();
	test::Board::castle<eng::Board>();
	test::Generator::test<eng::Board, eng::MoveGenerator<eng::Board>>();
	test::Generator::ep<eng::Board, eng::MoveGenerator<eng::Board>>();
	test::Generator::checks<eng::Board, eng::MoveGenerator<eng::Board>>();
}

/*TODO use concepts for template parameter*/
int main()
{
	run_tests();

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
