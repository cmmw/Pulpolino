/*
 * MoveGenerator.h
 *
 *  Created on: 05.05.2014
 *      Author: Christian
 */

#ifndef MOVEGENERATOR_H_
#define MOVEGENERATOR_H_

#include <vector>
#include "Board.h"

namespace eng
{

/* BOARD_T needs to provide the following
 *
 * Functions:
 *
 * BOARD_T::Piece_t get_piece(uint32_t idx)
 * BOARD_T::Color_t get_color(uint32_t idx)
 * bool has_moved(uint32_t idx)			      					returns false if there is a piece on the square and the piece has not been moved until now
 * BOARD_T::GenMov_t gen_mov(uint32_t from, uint32_t to);		build a GenMov_t with given arguments, needed to create a list of moves
 *
 *
 * Members/Enums:
 *
 * BOARD_T::WHITE, BOARD_T::BLACK
 *
 *
 * Typedefs:
 *
 * BOARD_T::Piece_t			(see BoardBase.h for information about BOARD_T::Piece_t)
 * BOARD_T::Color_t
 * BOARD_T::GenMov_t		the generator will generate a list of these move types
 *
 */

template<class BOARD_T>
class MoveGenerator
{
public:
	MoveGenerator() = default;
	~MoveGenerator() = default;

	MoveGenerator(const MoveGenerator&) = delete;
	MoveGenerator(MoveGenerator&&) = delete;
	MoveGenerator& operator=(const MoveGenerator&) = delete;

	void generate_moves(BOARD_T& board, std::vector<typename BOARD_T::GenMove_t>& moves, typename BOARD_T::Color_t c);

private:

	bool _slide[6] =
			{ false, false, true, true, true, false };

	int8_t _offsets[6] =
			{ 0, 8, 4, 4, 8, 8 };

	int8_t _offset[6][8] =
			{
					{ 0, 0, 0, 0, 0, 0, 0, 0 },
					{ -21, -19, -12, -8, 8, 12, 19, 21 }, /* KNIGHT */
					{ -11, -9, 9, 11, 0, 0, 0, 0 }, /* BISHOP */
					{ -10, -1, 1, 10, 0, 0, 0, 0 }, /* ROOK */
					{ -11, -10, -9, -1, 1, 9, 10, 11 }, /* QUEEN */
					{ -11, -10, -9, -1, 1, 9, 10, 11 } /* KING */
			}
	;

	int8_t _mailbox[120] =
			{
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, 0, 1, 2, 3, 4, 5, 6, 7, -1,
					-1, 8, 9, 10, 11, 12, 13, 14, 15, -1,
					-1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
					-1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
					-1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
					-1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
					-1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
					-1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1
			};

	int8_t _mailbox64[64] =
			{
					21, 22, 23, 24, 25, 26, 27, 28,
					31, 32, 33, 34, 35, 36, 37, 38,
					41, 42, 43, 44, 45, 46, 47, 48,
					51, 52, 53, 54, 55, 56, 57, 58,
					61, 62, 63, 64, 65, 66, 67, 68,
					71, 72, 73, 74, 75, 76, 77, 78,
					81, 82, 83, 84, 85, 86, 87, 88,
					91, 92, 93, 94, 95, 96, 97, 98
			};
};

} /* namespace eng */

#include "MoveGenerator.cpp"

#endif /* MOVEGENERATOR_H_ */
