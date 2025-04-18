#include "bitboards.h"
#include "utils.h"
#include "position.h"
#include "types.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace Engine {

std::vector<std::string> split(const std::string &s, const std::string &delimiter) {
    std::vector<std::string> tokens;
    size_t last = -1, next = 0, delimiterLength = delimiter.length();
    std::string token;

    while ((next = s.find(delimiter, last + 1)) != std::string::npos) {
        token = s.substr(last + delimiterLength, next - last - delimiterLength);
        tokens.push_back(token);
        last = next;
    }

    if (last != s.length() - 1) {
        token = s.substr(last + delimiterLength, std::string::npos);
        tokens.push_back(token);
    }

    return tokens;
}

Position::Position(std::string &fenString) : Position() {
    this->from_fen(fenString);
}

void Position::from_fen(std::string &fenString) {
    std::vector<std::string> sections = split(fenString, " ");

    // handle errors
    assert(sections.size() >= 6);
    /*
    if (tokens.size() != 6) {
        ErrorHandler::exitWithError(__FILE__, __FUNCTION__, __LINE__, "Wrong FEN format");
    }
    */

    // empty the board
    for (size_t square = Square::A1; square < Square::SquareNumber; ++square) {
        this->board[square] = Piece::NoPiece;
    }

    // fill the board with pieces
    Rank rank = Rank::Rank8;
    File file = File::FileA;
    for (const char &c : sections[0]) {
        Square currentSquare = Square(rank * 8 + file);

        if (c == '/') {
            file = File::FileA;
            rank = Rank(rank - 1);
        } else {
            if (is_alpha(c)) {
                Piece piece = piece_from_char(c);
                this->board[currentSquare] = piece;

                // set bitboards
                bitboard_t squareBitboard = Bitboard::from_square(currentSquare);
                this->bitboards[(piece & 0b111) + 1] |= squareBitboard;

                if (is_lower_case(c)) {
                    this->bitboards[BB_BLACK] |= squareBitboard;
                } else {
                    this->bitboards[BB_WHITE] |= squareBitboard;
                }

                file = File(file + 1);
            } else if (is_numerical(c)) {
                file = File(file + c - '0');
            }
        }
    }

    // active color
    this->sideToMove = (sections[1] == "w") ? Color::White : Color::Black;

    // castling rights
    this->castlingRights = CastlingRights::NoCastlingRights;

    if (sections[2] != "-") {
        for (const char &c : sections[2]) {
            char currentCastlingRights = CastlingRights::NoCastlingRights;

            if (to_lower_case(c) == 'k') {
                currentCastlingRights |= CastlingRights::KingSide;
            } else {
                currentCastlingRights |= CastlingRights::QueenSide;
            }

            if (is_lower_case(c)) {
                currentCastlingRights &= CastlingRights::BlackRights;
            } else {
                currentCastlingRights &= CastlingRights::WhiteRights;
            }

            this->castlingRights = (CastlingRights)(this->castlingRights | currentCastlingRights);
        }
    }

    // en passant target square
    this->enPassant = Square::SquareNumber;

    if (sections[3] != "-") {
        this->enPassant = square_from_string(sections[3]);
    }

    // halfmove clock
    this->halfmoveClock = std::stoi(sections[4]);

    // fullmove number
    this->fullmoveNumber = std::stoi(sections[5]);
}

std::string Position::to_fen(void) {
    std::stringstream fenString;
    unsigned int skip = 0;

    for (size_t rank = Rank::Rank8; rank < Rank::RankNumber; --rank) {
        for (size_t file = File::FileA; file < File::FileNumber; ++file) {
            size_t square = rank * 8 + file;

            if (this->board[square] == Piece::NoPiece) {
                ++skip;
            } else {
                if (skip > 0) {
                    fenString << skip;
                    skip = 0;
                }

                fenString << piece_to_char(this->board[square]);
            }
        }

        if (skip > 0) {
            fenString << skip;
            skip = 0;
        }

        if (rank != Rank::Rank1) fenString << "/";
    }

    fenString << " " << (this->sideToMove == Color::Black ? "b" : "w");
    fenString << " " << castling_rights_to_string(this->castlingRights);
    fenString << " " << square_to_string(this->enPassant);
    fenString << " " << this->halfmoveClock;
    fenString << " " << this->fullmoveNumber;

    return fenString.str();
}

void Position::display(void) {
    for (size_t rank = Rank::Rank8; rank < Rank::RankNumber; --rank) {
        for (size_t file = File::FileA; file < File::FileNumber; ++file) {
            std::cout << piece_to_char(this->board[rank * 8 + file]) << " ";
        }

        if (rank == Rank::Rank6) {
            std::cout << "side to move : " << (this->sideToMove == Color::Black ? "black" : "white");
        } else if (rank == Rank::Rank5) {
            std::cout << "reversible moves : " << this->halfmoveClock;
        } else if (rank == Rank::Rank4) {
            std::cout << "en passant : " << square_to_string(this->enPassant);
        } else if (rank == Rank::Rank3) {
            std::cout << "castling rights : " << castling_rights_to_string(this->castlingRights);
        } else if (rank == Rank::Rank2) {
            std::cout << "hash : " << this->hash;
        }

        std::cout << std::endl;
    }
}

void Position::display_bitboards(void) {
    static const std::string bbNames[BB_NUMBER] = {
        "Black Pieces",
        "White Pieces",
        "Pawns",
        "Knights",
        "Bishops",
        "Rooks",
        "Queens",
        "Kings",
    };

    Bitboard::display(this->bitboards[BB_BLACK], bbNames[BB_BLACK], '.', '*');
    Bitboard::display(this->bitboards[BB_WHITE], bbNames[BB_WHITE], '.', '*');

    for (size_t bitboard = BB_PAWN; bitboard < BB_NUMBER; bitboard++) {
        Bitboard::display(this->bitboards[bitboard], bbNames[bitboard], '.', piece_to_char(Piece(bitboard - 1)));
    }
}

} // namespace Engine
