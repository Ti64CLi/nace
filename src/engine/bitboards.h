#pragma once

#include "types.h"
#include <cstdint>
#include <string>
#include <vector>

namespace Engine {

typedef unsigned long long bitboard_t;

namespace Bitboard {

typedef struct {
    bitboard_t mask;
    bitboard_t magic;
    uint8_t shift;
    uint32_t offset;
} MagicEntry;

typedef struct {
    std::vector<MagicEntry> entries;
    std::vector<bitboard_t> attacks;
} MagicAttacks;

Square lsb(bitboard_t bb);
Square msb(bitboard_t bb);

bitboard_t empty(void);
bitboard_t universe(void);
bitboard_t from_square(const Square &square);

bool is_empty(const bitboard_t &bitboard);
bool is_not_empty(const bitboard_t &bitboard);
bool intersects(const bitboard_t &b1, const bitboard_t &b2);
bool is_set(const bitboard_t &bitboard, const Square &square);

bitboard_t intersect(const bitboard_t &b1, const bitboard_t &b2);
bitboard_t unite(const bitboard_t &b1, const bitboard_t &b2);
bitboard_t invert(const bitboard_t &bitboard);
bitboard_t shift(const bitboard_t &bitboard, char shift);

uint8_t count_ones(bitboard_t bb);

bitboard_t relevant_blockers(const PieceType pieceType, const Square square);
bitboard_t moves(const PieceType pieceType, const Square square, const bitboard_t blockers);

std::vector<Square> serialize(const bitboard_t &bitboard);

void display(const bitboard_t &bitboard, const std::string &bitboardName, const char &zeroChar, const char &oneChar);

} // namespace bitboard

} // namespace engine
