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

//				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//
//				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//				WHITE | PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK | PAWN,
//
//				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		};

Board::Board() :
		_color(WHITE)
{
	this->reset_board();
}

Board::Board(Color_t c)
{
	this->reset_board();
	this->_color = c;
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
	this->_color = WHITE;
	memcpy(this->_board, Board::init_board, sizeof(this->_board));
}

bool Board::move(const char* move)
{
	square_t from_row, from_file, to_row, to_file;
	from_file = move[0] - 'a';
	from_row = move[1] - '1';
	to_file = move[2] - 'a';
	to_row = move[3] - '1';

	return this->move(from_row * 8 + from_file, to_row * 8 + to_file);
}

bool Board::move(const GenMove_t& move)
{
	return this->move(move.from, move.to);
}

bool Board::move(uint8_t from, uint8_t to)
{
	/* TODO finish implementation (en passant, promoting, castle... etc) there are still some invalid moves...*/
	Move_t m;
	m.capture = this->_board[to];
	m.from = from;
	m.to = to;
	m.orig = this->_board[m.from];
	this->_board[m.to] = m.orig | MOVED;
	this->_board[m.from] = EMPTY;

//	static int capt = 0;
//	if (m.capture != EMPTY)
//	{
//		capt++;
//		g_log << "capture " << " " << capt << std::endl;
//	}

	if (this->_in_check(this->_color))
	{
		this->_board[to] = m.capture;
		this->_board[from] = m.orig;
		return false;
	}
	this->_history.push_back(m);
	this->_color = (this->_color == WHITE) ? BLACK : WHITE;

//	static int checks = 0;
//	if (this->_in_check(this->_color))
//	{
//		checks++;
//		g_log << "checks: " << checks << std::endl;
//	}

	return true;
}

bool Board::_in_check(Color_t c)
{
	for (int i = 0; i < 64; i++)
	{
		if (this->get_piece(i) == KING && this->get_color(i) == c)
		{
			if (this->_is_attacked(c, i))
				return true;
		}
	}
	return false;
}

bool Board::_is_attacked(Color_t c, uint8_t sq)
{
	for (int i = 0; i < 64; i++)
	{
		if (this->get_piece(i) != EMPTY && this->get_color(i) != c)
		{
			if (this->get_piece(i) == PAWN)
			{
				if (c == WHITE)
				{
					if (i - 9 == sq || i - 7 == sq)
						return true;
				}
				else
				{
					if (i + 9 == sq || i + 7 == sq)
					{
						return true;
					}
				}
			}
			else
			{
				Piece_t piece = this->get_piece(i);
				for (int j = 0; j < BoardBase::_offsets[piece]; j++)
				{
					for (int n = i;;)
					{
						n = BoardBase::_mailbox[BoardBase::_mailbox64[n] + BoardBase::_offset[piece][j]];
						if (n == sq)
							return true;
						if (n == -1 || this->get_piece(n) != EMPTY || !BoardBase::_slide[piece])
							break;
					}
				}
			}
		}
	}
	return false;
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
	this->_color = (this->_color == WHITE) ? BLACK : WHITE;
	return true;
}

} /* namespace eng */
