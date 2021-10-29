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

#include "gem.hpp"

blit::Point Gem::next_position(uint8_t x, uint8_t y) {
  uint8_t dest_x = Gem::SPRITE_SIZE * x;
  uint8_t dest_y = Gem::SPRITE_SIZE * y;

  position.x += sgn(dest_x - position.x);
  position.y += sgn(dest_y - position.y);

  return position;
}

blit::Rect Gem::next_sprite() {
  return blit::Rect((sprite_index << 1) + sprite_index, 0, Gem::SPRITE_SIZE >> 3, Gem::SPRITE_SIZE >> 3);
}
