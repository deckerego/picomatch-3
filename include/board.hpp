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
#include <cstdlib>
#include "gem.hpp"

struct Board {
  static const uint8_t ROWS = 9;
  static const uint8_t COLS = 10;
  static const uint32_t GAME_TIME = 30 * 1000;

  Gem* board[Board::COLS][Board::ROWS];
  uint32_t time_elapsed = 0;

  void initialize();
  void draw(blit::Surface screen);
  void update();

  void swap_left(std::pair<uint8_t, uint8_t> location);
  void swap_right(std::pair<uint8_t, uint8_t> location);
  void swap_up(std::pair<uint8_t, uint8_t> location);
  void swap_down(std::pair<uint8_t, uint8_t> location);

  uint8_t mark_matches();

  void serialize(std::pair<blit::Point, uint8_t> data[Board::COLS][Board::ROWS]);
  void deserialize(std::pair<blit::Point, uint8_t> data[Board::COLS][Board::ROWS]);

private:
  void swap(uint8_t origin_x, uint8_t origin_y, uint8_t dest_x, uint8_t dest_y);
  void remove(uint8_t x, uint8_t y);
};
