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

#include "cursor.hpp"

void Cursor::move_left() {
  position.x -= position.x >= Gem::SPRITE_SIZE ? Gem::SPRITE_SIZE : 0;
}

void Cursor::move_right() {
  position.x += position.x < (240 - Gem::SPRITE_SIZE) ? Gem::SPRITE_SIZE : 0;
}

void Cursor::move_up() {
  position.y -= position.y >= Gem::SPRITE_SIZE ? Gem::SPRITE_SIZE : 0;
}

void Cursor::move_down() {
  position.y += position.y < (216 - Gem::SPRITE_SIZE) ? Gem::SPRITE_SIZE : 0;
}
