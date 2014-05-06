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

template<class BOARD_T, class MOVGEN_T>
Engine<BOARD_T, MOVGEN_T>::Engine() :
		_stop(false), _quit(false)
{
	this->_go.lock();
}

template<class BOARD_T, class MOVGEN_T>
void Engine<BOARD_T, MOVGEN_T>::start()
{
	this->_run();
}

template<class BOARD_T, class MOVGEN_T>
void Engine<BOARD_T, MOVGEN_T>::go()
{
	this->_go.unlock();
}

template<class BOARD_T, class MOVGEN_T>
void Engine<BOARD_T, MOVGEN_T>::stop()
{
	this->_go.try_lock();
	this->_stop.store(true);
}

template<class BOARD_T, class MOVGEN_T>
void Engine<BOARD_T, MOVGEN_T>::quit()
{
	this->_quit.store(true);
	this->_stop.store(true);
	this->_go.unlock();
}

template<class BOARD_T, class MOVGEN_T>
void Engine<BOARD_T, MOVGEN_T>::_run()
{
	while (!_quit.load())
	{
		this->_go.lock();
		if (_quit.load())
		{
			break;
		}
		this->_start_calc();
	}
}

template<class BOARD_T, class MOVGEN_T>
void Engine<BOARD_T, MOVGEN_T>::_start_calc()
{
	this->_stop.store(false);
	while (!this->_stop.load())
	{
		g_log << "info string calculating..." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	if (!this->_quit.load())
		g_log << "bestmove a2a3" << std::endl;

	this->_stop.store(false);
}

template<class BOARD_T, class MOVGEN_T>
void Engine<BOARD_T, MOVGEN_T>::position(const std::string& pos)
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
			/* TODO finish implementation */
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
