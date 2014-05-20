/*
 * Board.h
 *
 *  Created on: 04.05.2014
 *      Author: Christian
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <cstdint>
#include <stack>
#include <string>

#include "BoardBase.h"

namespace eng
{

class Board: public BoardBase
{
public:

	typedef uint8_t square_t;

	class GenMove_t
	{
	public:
		GenMove_t() :
				_from(0), _to(0), _score(0), _promote(BoardBase::EMPTY)
		{

		}
		GenMove_t(int32_t from, int32_t to, int32_t score, Piece_t promote) :
				_from(static_cast<uint8_t>(from)), _to(static_cast<uint8_t>(to)), _score(score), _promote(promote)
		{
		}

		inline uint8_t from() const
		{
			return this->_from;
		}

		inline uint8_t to() const
		{
			return this->_to;
		}

		inline uint32_t score() const
		{
			return this->_score;
		}

		inline void score(uint32_t score)
		{
			this->_score = score;
		}

		inline Piece_t promote() const
		{
			return this->_promote;
		}

		bool operator>(const GenMove_t& m2) const
				{
			return this->_score > m2._score;
		}

	private:
		uint8_t _from;
		uint8_t _to;
		int32_t _score;
		Piece_t _promote;
	};

	enum Color_t
	{
		WHITE = 0 << 7, BLACK = 1 << 7
	};

	Board();
	Board(Color_t c);
	~Board() = default;

	Board(Board&&) = delete;
	Board(const Board&) = delete;
	Board& operator=(const Board&) = delete;

	void print();
	bool move(const char* move);
	bool move(const GenMove_t& move);
	bool move(int32_t from, int32_t to, Piece_t promote = EMPTY);
	void reset();
	void clear_history();
	bool take_back();
	void set_fen_pos(const char* fen);

	inline Color_t get_color(int32_t idx)
	{
		return static_cast<Color_t>(this->_board[idx] & COLOR);
	}

	inline Piece_t get_piece(int32_t idx)
	{
		return static_cast<Piece_t>(this->_board[idx] & PIECE);
	}

	inline GenMove_t gen_mov(int32_t from, int32_t to, int32_t score = 0, Piece_t p = EMPTY)
	{
		return
		{	from, to, score, p};
	}
	/*color of current player to move in this position*/
	inline Color_t get_color()
	{
		return this->_color;
	}

	/*check if color c is checked in current position*/
	bool in_check(Color_t c);
	/*check if color c's square sq is attacked*/
	bool is_attacked(Color_t c, int32_t sq);

	static std::string mov_to_str(const GenMove_t& mov);

private:

	struct Move_t
	{
		uint8_t from;
		uint8_t to;
		square_t capture;
		square_t orig;
	};

	enum Mask_t
	{
		COLOR = 1 << 7, DIR = 1 << 6, EP = 1 << 5, CASTLE = 1 << 4, MOVED = 1 << 3, PIECE = 7
	};

	square_t _board[64] =
			{ 0 };

	char _piece_code[12] =
			{ 'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k' };

	const static square_t init_board[];

	std::stack<Move_t> _history;
	/*Color to move in the current position*/
	Color_t _color;
	uint32_t _ply = 0;
	uint32_t _mov = 0;
	int32_t _ep = -1; /*fen string ep*/

}
;

} /* namespace eng */

#endif /* BOARD_H_ */
