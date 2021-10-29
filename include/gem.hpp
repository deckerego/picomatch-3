/**
 * PicoSystem Match 3
 * Copyright (C) 2021  DeckerEgo.net
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
**/

#ifndef LIB_GEM
#define LIB_GEM

#include <cstdint>
#include "32blit.hpp"

template <typename T> int sgn(T v) {
    return (T(0) < v) - (v < T(0));
}

struct Gem {
    static const uint8_t SPRITE_SIZE  = 24;
    static const uint8_t SPRITE_COUNT = 5;
    static const uint8_t NONE         = 0;

    uint8_t state = Gem::NONE;
    uint8_t sprite_index = 0;
    blit::Point position;

    Gem(blit::Point pos) : sprite_index(blit::random() % Gem::SPRITE_COUNT), position(pos) {};
    Gem(blit::Point pos, uint8_t idx) : sprite_index(idx), position(pos) {};

    blit::Point next_position(uint8_t x, uint8_t y);
    blit::Rect next_sprite();
};

#endif
