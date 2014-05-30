/*
 * Engine.h
 *
 *  Created on: 04.05.2014
 *      Author: Christian
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#define MIN (-300000)
#define MAX (300000)

#include <mutex>
#include <atomic>
#include <array>

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
	uint32_t _depth = 4;
	std::mutex _go;
	std::atomic<bool> _stop;
	std::atomic<bool> _quit;
	std::thread _input_th;

	BOARD_T _board;
	MOVGEN_T _movegen;
	EVAL_T _eval;

	void _uci_input_th();
	void _run();
	/*call when uci command position is received, will set position on the board, argument = received string from gui (including "position")*/
	void _position(const std::string& pos);

	/*algorithm specific stuff*/
	class LineInfo
	{
	public:
		LineInfo() = default;
		LineInfo(const LineInfo&) = delete;
		LineInfo(LineInfo&&) = delete;
		void operator=(LineInfo&) = delete;
		void add_move(const typename BOARD_T::GenMove_t &move)
		{
			this->_moves.push_back(move);
		}

		void reset()
		{
			this->_moves.clear();
			this->_mate = false;
			this->_stalemate = false;
			this->_follow_pv = false;
		}

		void operator<<(const LineInfo& other)
		{
			for (auto m : other._moves)
				this->_moves.push_back(m);
			this->_mate = other._mate;
			this->_stalemate = other._stalemate;
		}

		const std::vector<typename BOARD_T::GenMove_t>& moves()
		{
			return this->_moves;
		}

		void mate(bool b)
		{
			this->_mate = b;
		}

		void stalemate(bool b)
		{
			this->_stalemate = b;
		}

		bool mate()
		{
			return this->_mate;
		}

		bool stalemate()
		{
			return this->_stalemate;
		}

		bool follow_pv()
		{
			return this->_follow_pv;
		}

		void follow_pv(bool val)
		{
			this->_follow_pv = val;
		}

	private:
		std::vector<typename BOARD_T::GenMove_t> _moves;
		bool _mate = false;
		bool _stalemate = false;
		bool _follow_pv = false;
	};

	int32_t _think();
	int32_t _root_search(uint32_t);
	int32_t _search(uint32_t depth, int32_t alpha, int32_t beta, LineInfo& info);
	int32_t _quiescence(int32_t alpha, int32_t beta, LineInfo& info);

	typename BOARD_T::GenMove_t _bestmove;
	LineInfo _info;

	uint32_t _nodes = 0;

};

} /* namespace eng */

#include "impl/EngineImpl.h"

#endif /* ENGINE_H_ */
