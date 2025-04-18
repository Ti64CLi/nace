#pragma once

#include "types.h"
#include <cstdint>

namespace Engine {

class Move {
private:
    uint16_t data;

public:
    Move() = default;
    Move(const Move &move);
    Move(const Square &from, const Square &to, const Flag &flags);
    ~Move() = default;

    // getters and setters
    Square from(void) const;
    Square to(void) const;
    Flag flags(void) const;
    void set_origin(const Square &origin);
    void set_target(const Square &target);
    void set_flags(const Flag &flags);

    // utilities
    bool is_capture(void) const;
    bool is_promotion(void) const;

    // operators
    void operator=(Move move);
    bool operator==(Move move) const;
    bool operator!=(Move move) const;
};

} // namespace Engine