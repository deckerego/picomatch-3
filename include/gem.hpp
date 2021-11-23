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
  static const uint8_t REMOVE       = 1;
  static const uint8_t VANISH       = 2;
  static const uint8_t ASPLODE      = 4;
  static const uint8_t MARKED       = 8;

  static const uint8_t SHAPES       = 1;
  static const uint8_t FRUIT        = 2;
  static const uint8_t GEMS         = 4;
  static const uint8_t SPECIAL      = 8;

  uint8_t state = Gem::NONE;
  uint8_t type = Gem::SHAPES;
  uint8_t sprite_index = 0;
  blit::Point position;

  Gem(blit::Point pos, uint8_t type) : Gem(pos, type, blit::random() % Gem::SPRITE_COUNT) {};
  Gem(blit::Point pos, uint8_t type, uint8_t idx) : type(type), sprite_index(idx), position(pos) {
         if(type & Gem::SHAPES)  sprite_row = 3;
    else if(type & Gem::FRUIT)   sprite_row = 6;
    else if(type & Gem::GEMS)    sprite_row = 9;
  };

  void advance_to(uint8_t x, uint8_t y);
  void draw(blit::Surface screen);
  void vanish();
  void asplode();
  void special();

  bool left_of(Gem* of);
  bool up_of(Gem* of);
  bool eligible();
  bool deletable();
  bool equals(Gem* other);

private:
  uint8_t sprite_row = 0;
  uint8_t sprite_frame = 0;

  void draw_scale(blit::Surface screen, float scale);
  void draw_transform(blit::Surface screen, uint8_t sprite_idx, blit::SpriteTransform transformation);
};

#endif
