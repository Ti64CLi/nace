#include "bitboards.h"
#include "types.h"
#include <cassert>
#include <iostream>
#include <vector>

namespace Engine {

namespace Bitboard {

inline Square lsb(bitboard_t bb) {
    assert(bb);

    return Square(__builtin_ctzll(bb));
}

inline Square msb(bitboard_t bb) {
    assert(bb);

    return Square(63 - __builtin_clzll(bb));
}

inline bitboard_t empty(void) {
    return 0ULL;
}

inline bitboard_t universe(void) {
    return ~0ULL;
}

inline bitboard_t from_square(const Square &square) {
    return 1ULL << square;
}

inline bool is_empty(const bitboard_t &bitboard) {
    return bitboard == 0ULL;
}

inline bool is_not_empty(const bitboard_t &bitboard) {
    return bitboard != 0ULL;
}

inline bool intersects(const bitboard_t &b1, const bitboard_t &b2) {
    return is_not_empty(b1 & b2);
}

inline bool is_set(const bitboard_t &bitboard, const Square &square) {
    return intersects(bitboard, from_square(square));
}

inline bitboard_t intersect(const bitboard_t &b1, const bitboard_t &b2) {
    return b1 & b2;
}

inline bitboard_t unite(const bitboard_t &b1, const bitboard_t &b2) {
    return b1 | b2;
}

inline bitboard_t invert(const bitboard_t &bitboard) {
    return ~bitboard;
}

inline bitboard_t shift(const bitboard_t &bitboard, char shift) {
    if (shift > 0) {
        return bitboard << shift;
    } else {
        return bitboard >> (-shift);
    }
}

std::vector<Square> serialize(const bitboard_t &bitboard) {
    std::vector<Square> squareSet;

    for (size_t i = Square::A1; i < Square::SquareNumber; i++) {
        if (is_set(bitboard, Square(i))) {
            squareSet.push_back(Square(i));
        }
    }

    return squareSet;
}

void display(const bitboard_t &bitboard, const std::string &bitboardName, const char &zeroChar, const char &oneChar) {
    std::cout << "Bitboard '" << bitboardName << "' : " << std::endl;

    for (size_t rrank = Rank::Rank1; rrank < Rank::RankNumber; ++rrank) {
        for (size_t file = File::FileA; file < File::FileNumber; ++file) {
            std::cout << (intersects(bitboard, from_square(Square((Rank::Rank8 - rrank) * 8 + file))) ? oneChar : zeroChar) << " ";
        }

        std::cout << std::endl;
    }
}

} // namespace bitboard

} // namespace engine
