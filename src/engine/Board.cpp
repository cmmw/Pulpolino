/*
 * Board.cpp
 *
 *  Created on: 04.05.2014
 *      Author: Christian
 */

#include <iostream>
#include <cstring>
#include "Board.h"
#include "../globals.h"

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

Board::Board() :
		_color(WHITE)
{
	this->reset();
}

Board::Board(Color_t c)
{
	this->reset();
	this->_color = c;
}

void Board::print()
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

void Board::reset()
{
	this->_ply = 0;
	this->_mov = 0;
	this->_ep = -1;
	this->_color = WHITE;
	this->clear_history();
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

bool Board::move(uint8_t from, uint8_t to, Piece_t promote)
{
	/* TODO finish implementation (promoting... etc)*/
	Move_t m;
	Piece_t piece;
	bool moved = false;
	int8_t d = 0;
	m.from = from;
	m.to = to;
	m.capture = this->_board[to];
	m.orig = this->_board[from];

	piece = static_cast<Piece_t>(this->_board[from] & PIECE);
	if (piece == EMPTY)
		return false;

	/*en passant move*/
	if (piece == PAWN && this->get_piece(to) == BoardBase::EMPTY)
	{
		uint8_t row_fr = from >> 3; /*divison by 8*/
		uint8_t row_to = to >> 3;
		d = to - from; /*later points to the captured pawn*/
		if (d % 8 != 0)
		{
			if (row_fr == 4 && this->_color == WHITE && row_to == 5)
			{
				moved = true;
				d = d - 8;
			}
			else if (row_fr == 3 && this->_color == BLACK && row_to == 2)
			{
				moved = true;
				d = d + 8;
			}
		}

		if ((this->_board[from + d] & PIECE) == EMPTY || (this->_board[from + d] & COLOR) == this->_color)
		{
			moved = false;
		}

		if (moved)
		{
			if (this->_history.empty())
			{
				if (this->_ep != to)
					return false;
			}
			else
			{
				const Move_t& mh = this->_history.top();
				if (mh.to != from + d || (mh.from != mh.to - 2 * 8 && mh.from != mh.to + 2 * 8))
					return false;

			}
			if (d == 1)
				m.capture = this->_board[from + d] | EP | DIR;
			else
				m.capture = this->_board[from + d] | EP;

			this->_board[d + from] = EMPTY;
			this->_board[to] = this->_board[from];
			this->_board[from] = EMPTY;
		}
	}

	/*castle*/
	if (piece == KING && !moved)
	{
		switch (from)
		{
		case 4:
			if (to == 6)
			{
				if ((this->_board[7] & MOVED) || (this->_board[from] & MOVED))
					return false;

				if (this->is_attacked(this->_color, 4) || this->is_attacked(this->_color, 5) || this->is_attacked(this->_color, 6) || this->is_attacked(this->_color, 7))
					return false;

				moved = true;
				m.capture = this->_board[7] | CASTLE;
				this->_board[5] = this->_board[7] | MOVED;
				this->_board[7] = EMPTY;
				this->_board[to] = this->_board[from] | MOVED;
				this->_board[from] = EMPTY;
			}
			else if (to == 2)
			{
				if ((this->_board[0] & MOVED) || (this->_board[from] & MOVED))
					return false;

				if (this->is_attacked(this->_color, 4) || this->is_attacked(this->_color, 3) || this->is_attacked(this->_color, 2) || this->is_attacked(this->_color, 1) || this->is_attacked(this->_color, 0))
					return false;

				moved = true;
				m.capture = this->_board[0] | CASTLE;
				this->_board[3] = this->_board[0] | MOVED;
				this->_board[0] = EMPTY;
				this->_board[to] = this->_board[from] | MOVED;
				this->_board[from] = EMPTY;
			}
			break;
		case 60:
			if (to == 62)
			{
				if ((this->_board[63] & MOVED) || (this->_board[from] & MOVED))
					return false;

				if (this->is_attacked(this->_color, 60) || this->is_attacked(this->_color, 61) || this->is_attacked(this->_color, 62) || this->is_attacked(this->_color, 63))
					return false;

				moved = true;
				m.capture = this->_board[63] | CASTLE;
				this->_board[61] = this->_board[63] | MOVED;
				this->_board[63] = EMPTY;
				this->_board[to] = this->_board[from] | MOVED;
				this->_board[from] = EMPTY;
			}
			else if (to == 58)
			{
				if ((this->_board[56] & MOVED) || (this->_board[from] & MOVED))
					return false;

				if (this->is_attacked(this->_color, 60) || this->is_attacked(this->_color, 59) || this->is_attacked(this->_color, 58) || this->is_attacked(this->_color, 57) || this->is_attacked(this->_color, 56))
					return false;

				moved = true;
				m.capture = this->_board[56] | CASTLE;
				this->_board[59] = this->_board[56] | MOVED;
				this->_board[56] = EMPTY;
				this->_board[to] = this->_board[from] | MOVED;
				this->_board[from] = EMPTY;
			}
			break;
		}
	}

	if (!moved)
	{
		/*make normal or capture move*/
		this->_board[to] = m.orig | MOVED;
		this->_board[from] = EMPTY;
	}

	/*move is not allowed if we are in check after it, take it back*/
	if (this->in_check(this->_color))
	{
		if (m.capture & EP)
		{
			this->_board[d + from] = m.capture & ~(EP | DIR);
			m.capture = EMPTY;
		}
		this->_board[to] = m.capture;
		this->_board[from] = m.orig;
		return false;
	}

	this->_history.push(m);

	this->_color = (this->_color == WHITE) ? BLACK : WHITE;
	this->_ply++;
	if (this->_color == WHITE)
		this->_mov++;
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

	/*take back e.p. move*/
	if (m.capture & EP)
	{
		if (m.capture & DIR)
		{
			this->_board[m.from + 1] = m.capture & ~(EP | DIR);
		}
		else
		{
			this->_board[m.from - 1] = m.capture & ~EP;
		}
		this->_board[m.to] = EMPTY;
	}
	else if (m.capture & CASTLE)
	{
		this->_board[m.to] = EMPTY;
		switch (m.from)
		{
		case 4:
			this->_board[4] = KING | WHITE;
			if (m.to == 6)
			{
				this->_board[7] = ROOK | WHITE;
				this->_board[5] = EMPTY;
			}
			else
			{
				this->_board[0] = ROOK | WHITE;
				this->_board[3] = EMPTY;
			}
			break;
		case 60:
			this->_board[60] = KING | BLACK;
			if (m.to == 62)
			{
				this->_board[63] = ROOK | BLACK;
				this->_board[61] = EMPTY;
			}
			else
			{
				this->_board[56] = ROOK | BLACK;
				this->_board[59] = EMPTY;
			}
			break;
		}
	}
	else
	{
		this->_board[m.to] = m.capture;
	}
	this->_board[m.from] = m.orig;
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
	char from_r = _digit[(mov.from >> 3)];
	char to_f = _letter[(mov.to % 8)];
	char to_r = _digit[(mov.to >> 3)];

	return
	{	from_f, from_r, to_f, to_r};

}

void Board::set_fen_pos(const char* fen)
{
	uint32_t i, row, file;
	this->reset();
	memset(this->_board, EMPTY, sizeof(this->_board));
	i = 0;
	row = 7;
	file = 0;
	while (fen[i] != ' ')
	{
		square_t &p = this->_board[row * 8 + file];
		switch (fen[i])
		{
		case 'p':
			p = BLACK | PAWN;
			break;
		case 'r':
			p = BLACK | ROOK;
			break;
		case 'n':
			p = BLACK | KNIGHT;
			break;
		case 'b':
			p = BLACK | BISHOP;
			break;
		case 'q':
			p = BLACK | QUEEN;
			break;
		case 'k':
			p = BLACK | KING;
			break;

		case 'P':
			p = WHITE | PAWN;
			break;
		case 'R':
			p = WHITE | ROOK;
			break;
		case 'N':
			p = WHITE | KNIGHT;
			break;
		case 'B':
			p = WHITE | BISHOP;
			break;
		case 'Q':
			p = WHITE | QUEEN;
			break;
		case 'K':
			p = WHITE | KING;
			break;

		case '/':
			row--;
			file = -1;
			break;

		default:
			file += fen[i] - '1';
			break;

		}
		i++;
		file++;
	}

	i++;

	/*color group*/
	switch (fen[i])
	{
	case 'w':
		this->_color = WHITE;
		break;
	case 'b':
		this->_color = BLACK;
		break;
	default:
		g_log << "info string [FATAL] missing color in fen string" << std::endl;
		this->_color = WHITE;
		break;
	}

	/*castle group*/
	this->_board[7] |= MOVED;
	this->_board[0] |= MOVED;
	this->_board[63] |= MOVED;
	this->_board[56] |= MOVED;
	i += 2;
	while (fen[i] != ' ')
	{
		switch (fen[i])
		{
		case 'K':
			this->_board[7] &= ~MOVED;
			break;
		case 'Q':
			this->_board[0] &= ~MOVED;
			break;
		case 'k':
			this->_board[63] &= ~MOVED;
			break;
		case 'q':
			this->_board[56] &= ~MOVED;
			break;
		case '-':
			break;
		}
		i++;
	}

	/*en passant group*/
	i++;
	if (fen[i] != '-')
	{
		file = fen[i] - 'a';
		i++;
		row = fen[i] - '1';
		this->_ep = row * 8 + file;
	}

	/*ply number group*/
	i += 2;

	char tmp[16] =
			{ 0 };
	uint32_t j = 0;

	while (fen[i] != ' ')
	{
		tmp[j] = fen[i];
		j++;
		i++;
	}

	this->_ply = strtol(tmp, NULL, 10);

	/*move number group*/
	i++;
	memset(tmp, 0, sizeof(tmp));
	j = 0;
	while (fen[i] != 0)
	{
		tmp[j] = fen[i];
		j++;
		i++;
	}
	this->_mov = strtol(tmp, NULL, 10);

	if (i != strlen(fen))
	{
		g_log << "info string [WARNING] fen string parse problem " << std::endl;
	}
}

} /* namespace eng */
