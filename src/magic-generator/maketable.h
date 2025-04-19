#pragma once

#include "types.h"
#include <optional>
#include <vector>

uint16_t magic_index(const MagicEntry &entry, bitboard_t blockers);
std::optional<std::vector<bitboard_t>> make_table(PieceType pieceType, Square square, const MagicEntry &entry);
