/*
 * BoardBase.h
 *
 *  Created on: 05.05.2014
 *      Author: Christian
 */

#ifndef BOARDBASE_H_
#define BOARDBASE_H_

#include <stdint.h>

namespace eng
{

/*Every board has to use these 3 bit piece encodings so they can be used by the move generator for better performance.*/
class BoardBase
{
public:

	enum Piece_t
	{
		PAWN = 0, KNIGHT = 1, BISHOP = 2, ROOK = 3, QUEEN = 4, KING = 5, UNUSED = 6, EMPTY = 7
	};

};

} /* namespace eng */

#endif /* BOARDBASE_H_ */
