#pragma once

#include <cstdint>
#include <vector>

typedef uint64_t bitboard_t;

typedef struct {
    bitboard_t mask;
    bitboard_t magic;
    uint8_t shift;
    uint32_t offset;
} MagicEntry;

typedef struct {
    MagicEntry entry;
    std::vector<bitboard_t> attacks;
} MagicAttacks;

enum PieceType {
    Bishop,
    Rook,
};

enum Square {
    A1, B1, C1, D1, E1, F1, G1, H1, 
    A2, B2, C2, D2, E2, F2, G2, H2, 
    A3, B3, C3, D3, E3, F3, G3, H3, 
    A4, B4, C4, D4, E4, F4, G4, H4, 
    A5, B5, C5, D5, E5, F5, G5, H5, 
    A6, B6, C6, D6, E6, F6, G6, H6, 
    A7, B7, C7, D7, E7, F7, G7, H7, 
    A8, B8, C8, D8, E8, F8, G8, H8, 
    SquareNumber = 64,
};

enum File {
    FileA,
    FileB,
    FileC,
    FileD,
    FileE,
    FileF,
    FileG,
    FileH,
    FileNumber = 8,
};

enum Rank {
    Rank1,
    Rank2,
    Rank3,
    Rank4,
    Rank5,
    Rank6,
    Rank7,
    Rank8,
    RankNumber = 8,
};

inline Rank rank_from_square(Square s) {
    return Rank(s / 8);
}

inline File file_from_square(Square s) {
    return File(s % 8);
}

inline bool is_valid_square(Square square) {
    return (square < Square::SquareNumber) && (square >= Square::A1);
}

inline Square square_delta(Square square, const std::vector<char> &delta) {
    return Square(square + delta[0] + delta[1] * 8);
}

#define ABSS(x, y) ((x) > (y) ? ((x) - (y)) : ((y) - (x)))

inline unsigned int distance(File f1, File f2) {
    return ABSS(f1, f2);
}

inline unsigned int distance(Rank r1, Rank r2) {
    return ABSS(r1, r2);
}

inline unsigned int distance(Square sq1, Square sq2) {
    return distance(file_from_square(sq1), file_from_square(sq2)) + distance(rank_from_square(sq1), rank_from_square(sq2));
}
