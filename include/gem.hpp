#include <cstdint>
#include "32blit.hpp"

const uint8_t SPRITE_SIZE = 24;
const uint8_t SPRITE_COUNT = 5;

// Implementation of signum (returns 1, -1, 0)
template <typename T> int sgn(T v) {
    return (T(0) < v) - (v < T(0));
}

struct Gem {
    static const uint8_t NONE   = 0;

    uint8_t state = Gem::NONE;
    uint8_t sprite_index = 0;
    blit::Point position;

    Gem(uint8_t idx, blit::Point pos) : sprite_index(idx), position(pos) {};

    blit::Rect next_sprite();
};
