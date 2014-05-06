/*
 * Board.h
 *
 *  Created on: 04.05.2014
 *      Author: Christian
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <cstdint>
#include <vector>

#include "BoardBase.h"

namespace eng
{

class Board: public BoardBase
{
public:

	typedef uint8_t square_t;

	struct Move_t
	{
		uint8_t from;
		uint8_t to;
		square_t capture;
		square_t orig;
	};

	struct GenMove_t
	{
		uint8_t from;
		uint8_t to;
	};

	enum Color_t
	{
		WHITE = 0 << 7, BLACK = 1 << 7
	};

	enum Mask_t
	{
		COLOR = 1 << 7, CASTLE = 1 << 4, MOVED = 1 << 3, PIECE = 7
	};

	Board();
	Board(Color_t c);
	~Board() = default;

	Board(Board&&) = delete;
	Board(const Board&) = delete;
	Board& operator=(const Board&) = delete;

	void print_board();
	void move(const char* move);
	void move(const GenMove_t& move);
	void move(uint8_t from, uint8_t to);
	void reset_board();
	void clear_history();
	bool take_back();

	inline Color_t get_color(int8_t idx)
	{
		return static_cast<Color_t>(this->_board[idx] & COLOR);
	}

	inline Piece_t get_piece(uint8_t idx)
	{
		return static_cast<Piece_t>(this->_board[idx] & PIECE);
	}

	inline bool has_moved(uint8_t idx)
	{
		return static_cast<bool>(this->_board[idx] & MOVED) && ((this->_board[idx] & PIECE) != EMPTY);
	}

	inline GenMove_t gen_mov(uint8_t from, uint8_t to)
	{
		return {from, to};
	}

	inline Color_t get_color()
	{
		return this->_color;
	}

private:

	std::vector<Move_t> _history;
	Color_t _color;

	square_t _board[64] =
			{ 0 };

	char _piece_code[12] =
			{ 'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k' };

	const static square_t init_board[];
};

} /* namespace eng */

#endif /* BOARD_H_ */
