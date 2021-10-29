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

#include <cstdint>
#include "gem.hpp"

struct Board {
  static const uint8_t ROWS = 9;
  static const uint8_t COLS = 10;

  Gem* board[Board::COLS][Board::ROWS];

  void initialize();
  Gem* get(uint8_t x, uint8_t y);
  void swap(uint8_t origin_x, uint8_t origin_y, uint8_t dest_x, uint8_t dest_y);

  void serialize(std::pair<blit::Point, uint8_t> data[Board::COLS][Board::ROWS]);
  void deserialize(std::pair<blit::Point, uint8_t> data[Board::COLS][Board::ROWS]);
};