/*
 * Engine.h
 *
 *  Created on: 04.05.2014
 *      Author: Christian
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <mutex>
#include <atomic>

#include "MoveGenerator.h"

namespace eng
{

template<class BOARD_T, class MOVGEN_T, class EVAL_T>
class Engine
{
public:
	Engine();
	~Engine() = default;

	Engine(Engine&&) = delete;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine& other) = delete;

	/*starts the engines main loop*/
	void start();

	/*call when uci command go is received, will start the calculation of the current position*/
	void go();
	/*call when uci command stop is received, will stop the calculation*/
	void stop();

	/*call when uci command quit is received, will destroy the engine*/
	void quit();

	/*call when uci command position is received, will set position on the board, argument = received string from gui (including "position")*/
	void position(const std::string& pos);

	void print_board();

private:
	std::mutex _go;
	std::atomic<bool> _stop;
	std::atomic<bool> _quit;
	uint32_t _depth;

	BOARD_T _board;
	MOVGEN_T _movegen;
	EVAL_T eval;

	void _run();
	int32_t _think();
	int32_t _root_search(uint32_t);
	int32_t _search(uint32_t depth);


	typename BOARD_T::GenMove_t _bestmove;

};

} /* namespace eng */

#include "impl/EngineImpl.h"

#endif /* ENGINE_H_ */
