#include "magicgen.h"
#include "maketable.h"
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

uint8_t count_ones(bitboard_t bb) {
    uint8_t ones = 0;
    
    for (uint8_t idx = 0; idx < 64; ++idx) {
        ones += (bb >> idx) & 1;
    }

    return ones;
}

bitboard_t relevant_blockers(const PieceType pieceType, const Square square) {
    const std::vector<std::vector<char>> bishopDeltas = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    const std::vector<std::vector<char>> rookDeltas = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    const std::vector<std::vector<char>> &deltas = (pieceType == PieceType::Bishop) ? bishopDeltas : rookDeltas;
    bitboard_t blockers = 0ULL;
    Square current, next;

    for (const auto &delta : deltas) {
        current = square;

        while (is_valid_square(next = square_delta(current, delta)) && distance(current, next) <= 2) { // check the next square because border squares are irrelevant
            blockers |= 1ULL << current;
            current = next;
        }
    }

    blockers &= ~(1ULL << square); // remove initial square

    return blockers;
}

MagicResult find_magic(const PieceType pieceType, const Square square, std::mt19937_64 &mt) {
    MagicEntry entry;
    uint8_t indexBits;

    entry.mask = relevant_blockers(pieceType, square);
    indexBits = count_ones(entry.mask); // index bits is the number of blockers
    entry.shift = 64 - indexBits;

    for (;;) {
        entry.magic = mt() & mt() & mt();

        std::optional<std::vector<bitboard_t>> table = make_table(pieceType, square, entry);

        if (table.has_value()) {
            return {entry, table.value().size()};
        }
    }

    // never reached
    return {entry, 0};
}

void find_all_magics(const PieceType pieceType, std::mt19937_64 &mt) {
    size_t totalSize = 0;

    std::cout << "Engine::Bitboard::MagicEntry " << (pieceType == PieceType::Bishop ? "bishop" : "rook") << "Magics[Engine::Square::SquareNumber] = {" << std::endl;

    for (Square square = Square::A1; square < Square::SquareNumber; square = Square(square + 1)) {
        MagicResult result = find_magic(pieceType, square, mt);
        
        std::cout   << "\t{.mask = " << std::hex << std::setfill('0') << std::setw(16) << result.entry.mask 
                    << ", .magic = " << std::setfill('0') << std::setw(16) << result.entry.magic 
                    << ", .shift = " << std::dec << (int)result.entry.shift
                    << ", .offset = " << totalSize
                    << "}," << std::endl;
        
        totalSize += result.tableSize;
    }

    std::cout << "};" << std::endl;
}

bitboard_t string2bb(const std::string &s) {
    bitboard_t stringbb = 0ULL;
    char rank = 7, file = 0;
    char idx = 0;

    for (; rank != 0 || file != 7; ++idx, ++file) {
        if (s[idx] == 'X') {
            stringbb |= 1ULL << (rank * 8 + file);
        }

        if (file > 7) {
            file = 0;
            --rank;
        } else {
            ++idx;
        }
    }

    return stringbb;
}

int main() {
    std::mt19937_64 mt(time(nullptr));

    /*
        . . . X . . . X
        . . . . . . . .
        . . . X . . . .
        . . . . . . . .
        . . . . . . . X
        . . X . . . . .
        . . . X . X . .
        . . . . . . . .
    */

    //bitboard_t blockers = 0b0001000100000000000100000000000000000001001000000001010000000000;
    /*bitboard_t blockers = string2bb(
        ". . . X . . . X"
        ". . . . . . . ."
        ". . . X . . . ."
        ". . . . . . . ."
        ". . . . . . . X"
        ". . X . . . . ."
        ". . . X . X . ."
        ". . . . . . . ."
    );
    Square square = Square::D4;
    
    MagicAttacks bishopAttacks = find_magic(PieceType::Bishop, square, mt);

    display(blockers, "Blockers", '.', 'X');
    display(bishopAttacks.attacks[magic_index(bishopAttacks.entry, blockers)], "Moves for bishop in D4", '.', 'X');

    std::cout << "Mask : " << std::hex << bishopAttacks.entry.mask << " Magic : " << std::hex << bishopAttacks.entry.magic << " Shift : " << bishopAttacks.entry.shift << std::endl;
    std::cout << "Table size : " << bishopAttacks.attacks.size() << std::endl;*/

    find_all_magics(PieceType::Bishop, mt);
    find_all_magics(PieceType::Rook, mt);

    return 0;
}
