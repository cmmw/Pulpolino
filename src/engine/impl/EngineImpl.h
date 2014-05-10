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
		_stop(false), _quit(false), _depth(3)
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
		int32_t val;
		val = this->_think();
		g_log << "info string bestmove: " << BOARD_T::mov_to_str(this->_bestmove) << std::endl;
		g_log << "info string value: " << val << std::endl;
	}
}

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
int32_t Engine<BOARD_T, MOVGEN_T, EVAL_T>::_think()
{
	int32_t val;
	this->_stop.store(false);
	/* TODO call search in a iterative deepening manner (?)*/
	val = this->_search(this->_depth);
	this->_board.move(this->_bestmove);
	g_log << "bestmove " << BOARD_T::mov_to_str(this->_bestmove) << std::endl;
	this->_stop.store(false);
	return val;
}

/*negamax*/
template<class BOARD_T, class MOVGEN_T, class EVAL_T>
int32_t Engine<BOARD_T, MOVGEN_T, EVAL_T>::_search(uint32_t depth)
{
	/*TODO finish negamax search*/
	int32_t max = -99999999;
	int32_t val = max;

	if (depth == 0 || this->_stop.load())
	{
		if(this->_board.get_color() == BOARD_T::WHITE)
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
				if(depth == this->_depth)
				{
					this->_bestmove = *it;		//only needed on the highest recursion level, before exiting the function
				}
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
			this->_board.reset();
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

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
void Engine<BOARD_T, MOVGEN_T, EVAL_T>::print_board()
{
	this->_board.print();
}

} /* namespace eng */
