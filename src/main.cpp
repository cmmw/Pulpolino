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

#include "engine/Engine.h"
#include "engine/Board.h"
#include "engine/Evaluator.h"

#include "tests/Test.h"
#include "globals.h"

Logger g_log;

void run_tests()
{
//	test::Generator::perft<eng::Board, eng::MoveGenerator<eng::Board>>(5);
	test::Board::test<eng::Board>();
	test::Board::ep<eng::Board>();
	test::Board::checks<eng::Board>();
	test::Board::castle<eng::Board>();
	test::Generator::test<eng::Board, eng::MoveGenerator<eng::Board>>();
	test::Generator::ep<eng::Board, eng::MoveGenerator<eng::Board>>();
	test::Generator::checks<eng::Board, eng::MoveGenerator<eng::Board>>();
	test::Generator::calc<eng::Board, eng::MoveGenerator<eng::Board>>();
	g_log << "All tests ok" << std::endl;
}

/*TODO use concepts for template parameter*/
int main()
{

//	run_tests();
	eng::Engine<eng::Board, eng::MoveGenerator<eng::Board>, eng::Evaluator<eng::Board>> engine;
	engine.start();

	return 0;
}

