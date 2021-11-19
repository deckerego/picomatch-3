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

#ifndef LIB_PICOMATCH_GEM
#define LIB_PICOMATCH_GEM

#include <cstdint>
#include "32blit.hpp"
#include "config.hpp"

template <typename T> int sgn(T v) {
    return (T(0) < v) - (v < T(0));
}

struct Gem {
  static const uint8_t SPRITE_SIZE  = 24;
  static const uint8_t SPRITE_COUNT = 5;

  static const uint8_t NONE         = 0;
  static const uint8_t VANISH       = 1;
  static const uint8_t ASPLODE      = 2;

  static const uint8_t NORMAL       = 0;
  static const uint8_t HORIZ_LASER  = 1;
  static const uint8_t VERT_LASER   = 2;

  uint8_t state = Gem::NONE;
  uint8_t type = Gem::NORMAL;
  uint8_t sprite_index = 0;
  uint8_t sprite_frame = 0;
  blit::Point position;

  Gem(blit::Point pos) : type(Gem::NORMAL), sprite_index(blit::random() % Gem::SPRITE_COUNT), position(pos) {};
  Gem(blit::Point pos, uint8_t idx) : type(Gem::NORMAL), sprite_index(idx), position(pos) {};
  Gem(blit::Point pos, uint8_t type, uint8_t idx) : type(type), sprite_index(idx), position(pos) {};

  void advance_to(uint8_t x, uint8_t y);
  void draw(blit::Surface screen);
  void vanish();
  void asplode();

  bool left_of(Gem* of);
  bool up_of(Gem* of);
  bool eligible();
  bool deletable();

private:
  void draw_scale(blit::Surface screen, float scale);
};

#endif
