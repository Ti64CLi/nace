#pragma once

#include "bitboards.h"
#include "types.h"
#include <string>

#define BB_BLACK    0
#define BB_WHITE    1
#define BB_PAWN     2
#define BB_KNIGHT   3
#define BB_BISHOP   4
#define BB_ROOK     5
#define BB_QUEEN    6
#define BB_KING     7
#define BB_NUMBER   8

namespace Engine {

class Position {
private:
    Piece board[Square::SquareNumber];
    CastlingRights castlingRights;
    // en passant target squares
    Square enPassant;
    Color sideToMove;
    int halfmoveClock;
    int fullmoveNumber;

    unsigned long long hash;

    // bitboards
    // one for each color and one for each piece type
    bitboard_t bitboards[BB_NUMBER];

public:
    Position() = default;
    Position(std::string &fenString);
    ~Position() = default;

    // moves related functions


    // utilities
    void from_fen(std::string &fenString);
    std::string to_fen(void);

    void display(void);
    void display_bitboards(void);
};

} // namespace engine
