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
#include "Engine.h"
#include "../Globals.h"

namespace eng
{

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
Engine<BOARD_T, MOVGEN_T, EVAL_T>::Engine() :
		_stop(false), _quit(false), _depth(1)
{
	this->_go.lock();
}

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
void Engine<BOARD_T, MOVGEN_T, EVAL_T>::start()
{
	this->_run();
}

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
void Engine<BOARD_T, MOVGEN_T, EVAL_T>::go()
{
	this->_go.unlock();
}

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
void Engine<BOARD_T, MOVGEN_T, EVAL_T>::stop()
{
	this->_go.try_lock();
	this->_stop.store(true);
}

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
void Engine<BOARD_T, MOVGEN_T, EVAL_T>::quit()
{
	this->_quit.store(true);
	this->_stop.store(true);
	this->_go.unlock();
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
		this->_think();
	}
}

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
void Engine<BOARD_T, MOVGEN_T, EVAL_T>::_think()
{
	this->_stop.store(false);
	/* TODO call search in a iterative deepening manner (mabye?)*/
	this->_aicolor = this->_board.get_color();
	this->_search(this->_depth);
	g_log << "bestmove " << BOARD_T::mov_to_str(this->_bestmove) << std::endl;
	g_log << "info bestmove " << BOARD_T::mov_to_str(this->_bestmove) << std::endl;
	this->_stop.store(false);
}

/*minimax in negamax fashion*/
template<class BOARD_T, class MOVGEN_T, class EVAL_T>
int32_t Engine<BOARD_T, MOVGEN_T, EVAL_T>::_search(uint32_t depth)
{
	/*TODO finish negamax search*/
	int32_t val;
	int32_t max = -99999999;

	if (depth == 0 || this->_stop.load())
	{
		if (this->_board.get_color() == this->_aicolor)
			return eval(this->_board);
		return -eval(this->_board);
	}

	std::vector<typename BOARD_T::GenMove_t> moves;
	this->_movegen.gen_moves(this->_board, moves);
	for (typename std::vector<typename BOARD_T::GenMove_t>::iterator it = moves.begin(); it != moves.end(); it++)
	{
		if (this->_board.move(*it))
		{
			val = -this->_search(depth - 1);
			this->_board.take_back();
			if (val > max)
			{
				max = val;
				this->_bestmove = *it;		//only needed on the highest recursion level, before exiting the function
			}
		}
	}
	return max;
}

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
void Engine<BOARD_T, MOVGEN_T, EVAL_T>::position(const std::string& pos)
{
	std::vector<std::string> cmds;
	std::istringstream iss(pos);
	std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter<std::vector<std::string> >(cmds));
	for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); it++)
	{
		if (!it->compare("startpos"))
		{
			this->_board.reset_board();
		}
		else if (!it->compare("fen"))
		{
			/* TODO finish implementation of uci command fen */
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
	}
}

} /* namespace eng */
