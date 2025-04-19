#include "maketable.h"
#include "types.h"
#include <optional>

uint16_t magic_index(const MagicEntry &entry, bitboard_t blockers) {
    blockers = blockers & entry.mask;
    uint64_t hash = blockers * entry.magic;
    return hash >> entry.shift;
}

bitboard_t generate_moves(const PieceType pieceType, const Square square, const bitboard_t blockers) {
    const std::vector<std::vector<char>> bishopDeltas = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    const std::vector<std::vector<char>> rookDeltas = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    const std::vector<std::vector<char>> &deltas = (pieceType == PieceType::Bishop) ? bishopDeltas : rookDeltas;
    bitboard_t moves = 0ULL;
    Square current, next;

    for (auto &delta : deltas) {
        current = square;

        while (is_valid_square(next = square_delta(current, delta)) && distance(current, next) <= 2) {
            current = next;
            bitboard_t squarebb = 1ULL << current;
            moves |= squarebb;

            if (blockers & squarebb) {
                break;
            }
        }
    }

    return moves;
}

std::optional<std::vector<bitboard_t>> make_table(PieceType pieceType, Square square, const MagicEntry &entry) {
    std::vector<bitboard_t> table(1 << (64 - entry.shift));
    bitboard_t subset = 0ULL;

    for (;;) { // iterate over all subset of entry.mask
        bitboard_t moves = generate_moves(pieceType, square, subset); // generate all possible moves with to the 'subset' blockers
        uint16_t tableIndex = magic_index(entry, subset); // get table index from magic number and current blockers subset

        if (table[tableIndex] == 0ULL) { // no entry yet
            table[tableIndex] = moves;
        } else if (table[tableIndex] != moves) { // hash collision and different moves
            return std::optional<std::vector<bitboard_t>>();
        }

        // Carry-Ripler effect
        subset |= ~entry.mask;
        ++subset;
        subset &= entry.mask;

        if (subset == 0ULL) {
            break;
        }
    }

    return std::optional<std::vector<bitboard_t>>(table);
}