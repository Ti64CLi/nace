#include "move.h"

#define FROM_MASK   0x003f
#define TO_MASK     0x0fc0
#define FLAGS_MASK  0xf000

namespace Engine {

Move::Move(const Move &move) : data(move.data) {}

Move::Move(const Square &from, const Square &to, const Flag &flags) : data(from | (to << 6) | (flags << 12)) {}

// getters and setters
Square Move::from(void) const {
    return Square(this->data & FROM_MASK);
}

Square Move::to(void) const {
    return Square((this->data & TO_MASK) >> 6);
}

Flag Move::flags(void) const {
    return Flag((this->data & FLAGS_MASK) >> 12);
}

void Move::set_origin(const Square &origin) {
    this->data &= TO_MASK | FLAGS_MASK;
    this->data |= origin;
}

void Move::set_target(const Square &target) {
    this->data &= FROM_MASK | FLAGS_MASK;
    this->data |= (target << 6);
}

void Move::set_flags(const Flag &flags) {
    this->data &= FROM_MASK | TO_MASK;
    this->data |= flags;
}

// utilities
bool Move::is_capture(void) const {
    return (this->flags() & Flag::Capture) != 0;
}

bool Move::is_promotion(void) const {
    return (this->flags() & Flag::Promotion) != 0;
}

// operators
void Move::operator=(Move move) {
    this->data = move.data;
}

bool Move::operator==(Move move) const {
    return this->data == move.data;
}

bool Move::operator!=(Move move) const {
    return this->data != move.data;
}

} // namespace Engine