/*
 * Board.cpp
 *
 *  Created on: 04.05.2014
 *      Author: Christian
 */

#include <iostream>
#include "Board.h"
#include "../Globals.h"

namespace eng
{

const uint8_t Board::init_board[] =
		{
				ROOK | WHITE, KNIGHT | WHITE, BISHOP | WHITE, QUEEN | WHITE, KING | WHITE, BISHOP | WHITE, KNIGHT | WHITE, ROOK | WHITE,
				PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE,
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK,
				ROOK | BLACK, KNIGHT | BLACK, BISHOP | BLACK, QUEEN | BLACK, KING | BLACK, BISHOP | BLACK, KNIGHT | BLACK, ROOK | BLACK
		};

Board::Board()
{
	this->reset_board();
}

void Board::print_board()
{
	uint8_t piece;
	uint8_t color;
	for (int i = 7; i >= 0; i--)
	{
		for (int j = 0; j < 8; j++)
		{
			piece = this->_board[j + i * 8];
			switch (piece & PIECE)
			{
			case UNUSED:
				g_log << "U";
				break;
			case EMPTY:
				g_log << " ";
				break;
			default:
				color = (piece & COLOR) ? 1 : 0;
				g_log << this->_piece_code[(piece & PIECE) + (color * 6)];
				break;
			}
		}
		g_log << std::endl;
	}
}

void Board::reset_board()
{
	memcpy(this->_board, Board::init_board, sizeof(this->_board));
}

void Board::move(const char* move)
{
	square_t from_row, from_file, to_row, to_file;
	from_file = move[0] - 'a';
	from_row = move[1] - '1';
	to_file = move[2] - 'a';
	to_row = move[3] - '1';

	this->move(from_row * 8 + from_file, to_row * 8 + to_file);
}

void Board::move(const GenMove_t& move)
{
	this->move(move.from, move.to);
}

void Board::move(uint8_t from, uint8_t to)
{
	/* TODO finish implementation (e.p., promotion, castle... etc) */
	Move_t m;
	m.capture = this->_board[to];
	m.from = from;
	m.to = to;
	m.orig = this->_board[m.from];
	this->_board[m.to] = m.orig | MOVED;
	this->_board[m.from] = EMPTY;
	this->_history.push_back(m);
}

void Board::clear_history()
{
	this->_history.clear();
}

bool Board::take_back()
{
	if (this->_history.empty())
		return false;

	Move_t m = this->_history.back();
	this->_history.pop_back();
	this->_board[m.from] = m.orig;
	this->_board[m.to] = m.capture;
	return true;
}

} /* namespace eng */
