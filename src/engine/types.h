#pragma once

#include "utils.h"
#include <cassert>
#include <string>
#include <sstream>
#include <vector>

namespace Engine {

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

enum Color {
    Black,
    White,
    ColorNumber = 2,
};

enum PieceType {
    NoPieceType,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    PieceTypeNumber = 8,
};

enum Piece {
    NoPiece,
    BPawn       = PieceType::Pawn,
    BKnight,
    BBishop,
    BRook,
    BQueen,
    BKing,
    WPawn       = PieceType::Pawn + 8,
    WKnight,
    WBishop,
    WRook,
    WQueen,
    WKing,
    PieceNumber = 16,
};

enum CastlingRights {
    NoCastlingRights,
    BKingSide,
    BQueenSide              = CastlingRights::BKingSide << 1,
    WKingSide               = CastlingRights::BKingSide << 2,
    WQueenSide              = CastlingRights::BKingSide << 3,
    KingSide                = CastlingRights::BKingSide | CastlingRights::WKingSide,
    QueenSide               = CastlingRights::BQueenSide | CastlingRights::WQueenSide,
    BlackRights             = CastlingRights::BKingSide | CastlingRights::BQueenSide,
    WhiteRights             = CastlingRights::WKingSide | CastlingRights::WQueenSide,
    AllRights               = CastlingRights::BlackRights | CastlingRights::WhiteRights,
    CastlingRightsNumber    = 16,
};

enum PromotedPiece {
    PromotedKnight  = 0b00,
    PromotedBishop  = 0b01,
    PromotedRook    = 0b10,
    PromotedQueen   = 0b11,
};

enum Flag {
    Quiet           = 0b0000,
    DoublePawnPush  = 0b0001,
    KingCastle      = 0b0010,
    QueenCastle     = 0b0011,
    Capture         = 0b0100,
    EnPassant       = 0b0101,
    Promotion       = 0b1000,
    KnightPromotion = Promotion | PromotedKnight,
    BishopPromotion = Promotion | PromotedBishop,
    RookPromotion   = Promotion | PromotedRook,
    QueenPromotion  = Promotion | PromotedQueen,
};

typedef struct {
    CastlingRights castlingRights;
    Square enPassant;
    int halfmoveClock;
} UndoState;

/*
Piece piece_from_char(unsigned char pieceChar);
unsigned char piece_to_char(Piece piece);
//CastlingRights castlingRightsFromString(std::string castlingRightsString);
std::string castling_rights_to_string(CastlingRights castlingRights);
std::string square_to_string(Square square);
Square square_from_string(std::string squareName);
*/

inline Rank rank_from_square(Square s) {
    return Rank(s / 8);
}

inline File file_from_square(Square s) {
    return File(s % 8);
}

static const std::string pieceChars = ".pnbrqk";

inline Piece piece_from_char(unsigned char pieceChar) {
    assert(is_alpha(pieceChar) || pieceChar == '.');

    Color pieceColor = is_upper_case(pieceChar) ? Color::White : Color::Black;
    pieceChar = to_lower_case(pieceChar);
    
    for(size_t i = 1; i < pieceChars.length(); ++i) {
        if ((unsigned char)pieceChars[i] == pieceChar) {
            return Piece(i | (pieceColor << 3));
        }
    }

    return Piece::NoPiece;
}

inline unsigned char piece_to_char(Piece piece) {
    unsigned char pieceChar;

    pieceChar = pieceChars[piece & 0b111];

    if (piece & 8) {// white color
        pieceChar = to_upper_case(pieceChar);
    }

    return pieceChar;
}

inline std::string castling_rights_to_string(CastlingRights castlingRights) {
    std::stringstream castlingRightsString;

    if (castlingRights & CastlingRights::WKingSide) {
        castlingRightsString << "K";
    }
    if (castlingRights & CastlingRights::WQueenSide) {
        castlingRightsString << "Q";
    }
    if (castlingRights & CastlingRights::BKingSide) {
        castlingRightsString << "k";
    }
    if (castlingRights & CastlingRights::BQueenSide) {
        castlingRightsString << "q";
    }

    return castlingRightsString.str();
}

inline std::string square_to_string(Square square) {
    if (square == Square::SquareNumber) {
        return "-";
    }

    std::string squareString = "a1";

    squareString[0] += square % 8;
    squareString[1] += square / 8;

    return squareString;
}

inline Square square_from_string(std::string squareName) {
    if (squareName == "-") {
        return Square::SquareNumber;
    }

    assert(squareName.length() == 2);
    assert(is_alpha(squareName[0]));
    assert(is_numerical(squareName[1]));

    return Square((to_lower_case(squareName[0]) - 'a') + (squareName[1] - '1') * 8);
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

} // namespace engine
