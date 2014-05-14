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

private:
	std::mutex _go;
	std::atomic<bool> _stop;
	std::atomic<bool> _quit;
	uint32_t _depth;

	BOARD_T _board;
	MOVGEN_T _movegen;
	EVAL_T _eval;

	void _run();
	int32_t _think();
	int32_t _root_search(uint32_t);
	int32_t _search(uint32_t depth, int32_t alpha, int32_t beta);
	std::thread _input_th;

	typename BOARD_T::GenMove_t _bestmove;

	void uci_input_th();

	/*call when uci command position is received, will set position on the board, argument = received string from gui (including "position")*/
	void _position(const std::string& pos);

};

} /* namespace eng */

#include "impl/EngineImpl.h"

#endif /* ENGINE_H_ */
