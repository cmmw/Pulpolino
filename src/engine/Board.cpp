/*
 * Board.cpp
 *
 *  Created on: 04.05.2014
 *      Author: Christian
 */

#include <iostream>
#include <cstring>
#include "Board.h"
#include "../Globals.h"

namespace eng
{

const uint8_t Board::init_board[] =
		{

//				ROOK | WHITE, KNIGHT | WHITE, BISHOP | WHITE, QUEEN | WHITE, KING | WHITE, BISHOP | WHITE, KNIGHT | WHITE, ROOK | WHITE,
//				PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE,
//				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//				PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK,
//				ROOK | BLACK, KNIGHT | BLACK, BISHOP | BLACK, QUEEN | BLACK, KING | BLACK, BISHOP | BLACK, KNIGHT | BLACK, ROOK | BLACK

				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,

				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				EMPTY, BLACK | PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				EMPTY, BLACK | PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,

				WHITE | PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
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
		g_log << std::endl << "-----------------" << std::endl;
		g_log << "|";
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
			g_log << "|";
		}

	}
	g_log << std::endl << "-----------------" << std::endl;
}

void Board::reset_board()
{
	this->_color = WHITE;
	memcpy(this->_board, Board::init_board, sizeof(this->_board));
}

bool Board::move(const char* move)
{
	square_t from_row, from_file, to_row, to_file;
	Piece_t p = EMPTY;
	from_file = move[0] - 'a';
	from_row = move[1] - '1';
	to_file = move[2] - 'a';
	to_row = move[3] - '1';
	if (strlen(move) == 5)
	{
		switch (move[4])
		{
		case 'q':
			p = QUEEN;
			break;
		case 'n':
			p = KNIGHT;
			break;
		case 'b':
			p = BISHOP;
			break;
		case 'r':
			p = ROOK;
			break;
		default:
			g_log << "info string [FATAL] Promoting to unknown piece: " << move[4] << std::endl;
		}
	}
	return this->move(from_row * 8 + from_file, to_row * 8 + to_file, p);
}

bool Board::move(const GenMove_t& move)
{
	return this->move(move.from, move.to, move.promote);
}

bool Board::move(uint8_t from, uint8_t to, Piece_t promotoe)
{
	/* TODO finish implementation (check for legal castle move. Promoting... etc) there are still some invalid moves...*/
	Move_t m;
	m.from = from;
	m.to = to;

	/*en passant*/
	int32_t d = from - to;
	if (d < 0)
		d = to - from;
	if ((from & PIECE) == PAWN && (d == 7 || d == 9))
	{
		m.orig = this->_board[from];
		this->_board[to] = this->_board[from];
		this->_board[from] = EMPTY;

		if ((from & COLOR) == WHITE)
			d = d - 8;
		else
			d = 8 - d;

		if (!this->_history.empty())
		{
			const Move_t& top = this->_history.top();
			if (top.from != from || top.to != to)
			{
				return false;
			}
		}
		else
		{
			/*We do not allow EP if there is no history (generated position with fen string)*/
			return false;
		}

		m.capture = this->_board[from + d] | EP;
		this->_board[from + d] = EMPTY;

		/*move is not allowed if we are in check after it*/
		if (this->in_check(this->_color))
		{
			this->_board[from + d] = m.capture & (EP ^ 1);
			this->_board[from] = m.orig;
			this->_board[to] = EMPTY;
			return false;
		}
		this->_history.push(m);
		this->_color = (this->_color == WHITE) ? BLACK : WHITE;
		return true;
	}

	m.capture = this->_board[to];
	m.orig = this->_board[from];

	/*Promoting*/
	int8_t row_fr, row_to;
	row_fr = from / 8;
	row_to = to / 8;
	if (this->_color == WHITE && row_fr == 6 && row_to == 7 && promotoe == EMPTY)
	{
		return false;
	}

	if (this->_color == BLACK && row_fr == 1 && row_to == 0 && promotoe == EMPTY)
	{
		return false;
	}

	if (promotoe != EMPTY)
	{
		this->_board[from] = EMPTY;
		this->_board[to] = promotoe;
	}
	else
	{
		/*make normal or capture move*/
		this->_board[to] = m.orig | MOVED;
		this->_board[from] = EMPTY;
	}

	/*move is not allowed if we are in check after it*/
	if (this->in_check(this->_color))
	{
		this->_board[to] = m.capture;
		this->_board[from] = m.orig;
		return false;
	}

	this->_history.push(m);
	this->_color = (this->_color == WHITE) ? BLACK : WHITE;
	return true;
}

bool Board::in_check(Color_t c)
{
	for (int i = 0; i < 64; i++)
	{
		if (this->get_piece(i) == KING && this->get_color(i) == c)
		{
			if (this->is_attacked(c, i))
				return true;
		}
	}
	return false;
}

bool Board::is_attacked(Color_t c, uint8_t sq)
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
	this->_history = std::stack<Move_t>();
}

bool Board::take_back()
{
	if (this->_history.empty())
		return false;

	Move_t m = this->_history.top();
	this->_history.pop();
	this->_board[m.from] = m.orig;
	this->_board[m.to] = m.capture;
	this->_color = (this->_color == WHITE) ? BLACK : WHITE;
	return true;
}

std::string Board::mov_to_str(const GenMove_t& mov)
{
	static char _letter[8] =
			{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
	static char _digit[8] =
			{ '1', '2', '3', '4', '5', '6', '7', '8' };

	char from_f = _letter[(mov.from % 8)];
	char from_r = _digit[(mov.from / 8)];
	char to_f = _letter[(mov.to % 8)];
	char to_r = _digit[(mov.to / 8)];

	return
	{	from_f, from_r, to_f, to_r};

}

} /* namespace eng */
