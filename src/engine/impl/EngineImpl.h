/*
 * Engine.cpp
 *
 *  Created on: 04.05.2014
 *      Author: Christian
 */

#include <thread>
#include <iterator>
#include <sstream>
#include <vector>
#include "../Engine.h"
#include "../../globals.h"

namespace eng
{

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
Engine<BOARD_T, MOVGEN_T, EVAL_T>::Engine() :
		_stop(false), _quit(false), _depth(6)
{
	this->_go.lock();
	_input_th = std::thread(&Engine<BOARD_T, MOVGEN_T, EVAL_T>::uci_input_th, this);
}

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
void Engine<BOARD_T, MOVGEN_T, EVAL_T>::start()
{
	this->_run();
}

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
void Engine<BOARD_T, MOVGEN_T, EVAL_T>::_run()
{
	while (!_quit.load())
	{
		this->_go.lock();
		if (_quit.load())
		{
			break;
		}
		int32_t val;
		val = this->_think();
		g_log << "info string bestmove: " << BOARD_T::mov_to_str(this->_bestmove) << std::endl;
		g_log << "info string value: " << val << std::endl;
	}
	this->_input_th.join();
}

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
int32_t Engine<BOARD_T, MOVGEN_T, EVAL_T>::_think()
{
	/* TODO call search in a iterative deepening manner (?)*/
	int32_t val;
	this->_stop.store(false);
	val = this->_root_search(this->_depth);
	this->_board.move(this->_bestmove);
	g_log << "bestmove " << BOARD_T::mov_to_str(this->_bestmove) << std::endl;
	this->_stop.store(false);
	return val;
}

/*negamax with alpha beta pruning*/
template<class BOARD_T, class MOVGEN_T, class EVAL_T>
int32_t Engine<BOARD_T, MOVGEN_T, EVAL_T>::_root_search(uint32_t depth)
{
	int32_t val, alpha = -250000, beta = 250000;
	std::vector<typename BOARD_T::GenMove_t> moves;
	this->_movegen.gen_moves(this->_board, moves);
	for (const auto &it : moves)
	{
		if (!this->_board.move(it))
			continue;

		val = -this->_search(depth - 1, -beta, -alpha);
		this->_board.take_back();
		if (val > alpha)
		{
			alpha = val;
			this->_bestmove = it;
		}
	}
	return alpha;
}

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
int32_t Engine<BOARD_T, MOVGEN_T, EVAL_T>::_search(uint32_t depth, int32_t alpha, int32_t beta)
{
	int32_t val;
	std::vector<typename BOARD_T::GenMove_t> moves;

	if (depth == 0 || this->_stop.load())
		return this->_eval(this->_board);

	this->_movegen.gen_moves(this->_board, moves);
	for (const auto &it : moves)
	{
		if (!this->_board.move(it))
			continue;

		val = -this->_search(depth - 1, -beta, -alpha);
		this->_board.take_back();
		if (val >= beta)
			return beta;
		if (val > alpha)
			alpha = val;
	}
	return alpha;
}

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
void Engine<BOARD_T, MOVGEN_T, EVAL_T>::_position(const std::string& pos)
{
	std::vector<std::string> cmds;
	std::istringstream iss(pos);
	std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter<std::vector<std::string> >(cmds));
	std::vector<std::string>::iterator it = cmds.begin();
	it++;
	for (; it != cmds.end(); it++)
	{
		if (!it->compare("startpos"))
		{
			this->_board.reset();
		}
		else if (!it->compare("fen"))
		{
			this->_board.set_fen_pos(pos.c_str() + 13);
			it += 6;
		}
		else if (!it->compare("moves"))
		{
			for (it++; it != cmds.end(); it++)
			{
				this->_board.move(it->c_str());
			}
			this->_board.clear_history();
			break;
		}
		else
		{
			g_log << "info string unknown position string: " << *it << std::endl;
		}
	}
}

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
void Engine<BOARD_T, MOVGEN_T, EVAL_T>::uci_input_th()
{
	/*todo: finish implementation of uci commands*/
	while (true)
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
			_position(cmd);
		}
		else if (!cmd.compare(0, 2, "go"))
		{
			if (cmd.length() > 2 && cmd.substr(3).compare("depth"))
			{
				int32_t d = strtol(cmd.c_str() + 9, NULL, 10);
				if (d > 0)
				{
					this->_depth = d;
					g_log << "info string set depth to " << this->_depth << std::endl;
				}
			}
			this->_go.unlock();
		}
		else if (!cmd.compare("stop"))
		{
			this->_go.try_lock();
			this->_stop.store(true);
		}
		else if (!cmd.compare("ponderhit"))
		{

		}
		else if (!cmd.compare("quit"))
		{
			this->_quit.store(true);
			this->_stop.store(true);
			this->_go.unlock();
			break;
		}
		else if (!cmd.compare("print"))
		{
			this->_board.print();
		}
		else
		{
			g_log << "info string unknown cmd" << std::endl;
		}

	}
}

} /* namespace eng */
