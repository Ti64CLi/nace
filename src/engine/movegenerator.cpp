#include "bitboards.h"
#include "movegenerator.h"
#include "magics.h"
#include "types.h"

#include <iomanip>
#include <ios>
#include <iostream>
#include <vector>

namespace Engine {

static bitboard_t rookMovesTable[rookMovesTotalSize];
static bitboard_t bishopMovesTable[bishopMovesTotalSize];

uint16_t magic_index(const Bitboard::MagicEntry &entry, bitboard_t blockers) {
    blockers = blockers & entry.mask;
    uint64_t hash = blockers * entry.magic;
    return hash >> entry.shift;
}

void make_table(const PieceType pieceType, const Square square, const Bitboard::MagicEntry &entry, bitboard_t *table) {
    bitboard_t subset = 0ULL;

    for (;;) {
        bitboard_t generatedMoves = moves(pieceType, square, subset);
        uint32_t tableIndex = magic_index(entry, subset);

        if (table[tableIndex] == 0ULL) {
            table[tableIndex] = generatedMoves;
        } else if (table[tableIndex] != generatedMoves) {
            std::cerr   << "Invalid magic number for square '" << square_to_string(square) 
                        << "' (" << std::hex << std::setfill('0') << std::setw(16) << entry.magic
                        << ") : hash collision for the following subset :" << std::endl;
            Bitboard::display(subset, "blockers", '.', 'X');

            exit(EXIT_FAILURE);
        }

        subset |= ~entry.mask;
        ++subset;
        subset &= entry.mask;

        if (subset == 0ULL) {
            break;
        }
    }
}

void initialize_magic_bitboards() {
    // generate moves for rook & queen
    for (Square square = Square::A1; square < Square::SquareNumber; square = Square(square + 1)) {
        Bitboard::MagicEntry entry = rookMagics[square];
        bitboard_t *rookTablePtr = rookMovesTable + entry.offset;

        make_table(PieceType::Rook, square, entry, rookTablePtr);
    }

    // generate moves for bishop & queen
    for (Square square = Square::A1; square < Square::SquareNumber; square = Square(square + 1)) {
        Bitboard::MagicEntry entry = bishopMagics[square];
        bitboard_t *bishopTablePtr = bishopMovesTable + entry.offset;

        make_table(PieceType::Bishop, square, entry, bishopTablePtr);
    }
}

// for sliding pieces only (for now ?)
bitboard_t relevant_blockers(const PieceType pieceType, const Square square) {
    const std::vector<std::vector<char>> bishopDeltas = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    const std::vector<std::vector<char>> rookDeltas = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    const std::vector<std::vector<char>> &deltas = (pieceType == PieceType::Bishop) ? bishopDeltas : rookDeltas;
    bitboard_t blockers = Bitboard::empty();
    Square current, next;

    for (const auto &delta : deltas) {
        current = square;

        while (is_valid_square(next = square_delta(current, delta)) && distance(current, next) <= 2) { // check the next square because border squares are irrelevant
            blockers |= Bitboard::from_square(current);
            current = next;
        }
    }

    blockers &= ~Bitboard::from_square(square); // remove initial square

    return blockers;
}

bitboard_t moves(const PieceType pieceType, const Square square, const bitboard_t blockers) {
    const std::vector<std::vector<char>> bishopDeltas = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    const std::vector<std::vector<char>> rookDeltas = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    const std::vector<std::vector<char>> &deltas = (pieceType == PieceType::Bishop) ? bishopDeltas : rookDeltas;
    bitboard_t moves = Bitboard::empty();
    Square current, next;

    for (auto &delta : deltas) {
        current = square;

        while (is_valid_square(next = square_delta(current, delta)) && distance(current, next) <= 2) {
            current = next;
            bitboard_t squarebb = Bitboard::from_square(current);
            moves |= squarebb;

            if (blockers & squarebb) {
                break;
            }
        }
    }

    return moves;
}

}
