#pragma once

#include "bitboards.h"

namespace Engine {

// call this when starting the engine
void initialize_magic_bitboards(void);

bitboard_t relevant_blockers(const PieceType pieceType, const Square square);
bitboard_t moves(const PieceType pieceType, const Square square, const bitboard_t blockers);



}
