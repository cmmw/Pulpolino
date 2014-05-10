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
 * BOARD_T::Piece_t get_piece(uint8_t idx)
 * BOARD_T::Color_t get_color(uint8_t idx)
 * BOARD_T::Color_t get_color()									returns the color which is to move
 * BOARD_T::GenMov_t gen_mov(uint8_t from, uint8_t to);		build a GenMov_t with given arguments, needed to create a list of moves
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

	void gen_moves(BOARD_T& board, std::vector<typename BOARD_T::GenMove_t>& moves);

};

} /* namespace eng */

#include "impl/MoveGeneratorImpl.h"

#endif /* MOVEGENERATOR_H_ */
