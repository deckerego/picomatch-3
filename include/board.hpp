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
#include "config.hpp"
#include "gem.hpp"
#include "cursor.hpp"

struct Board {
  static const uint8_t  ROWS      = 9;
  static const uint8_t  COLS      = 10;

  static const uint8_t  NONE      = 0;
  static const uint8_t  CLEAR     = 1;
  static const uint8_t  INIT      = 2;

  Gem* board[Board::COLS][Board::ROWS];
  Cursor cursor = Cursor();
  uint8_t state = Board::NONE;
  uint8_t gem_type = Gem::SHAPES;
  uint32_t time_elapsed = 0;

  void initialize();
  void clear();
  bool cleared();

  void draw(blit::Surface screen);
  void press(blit::ButtonState &buttons);
  void update(uint32_t time);
  uint8_t mark_matches();

  void serialize(std::pair<blit::Point, uint8_t> data[Board::COLS][Board::ROWS]);
  void deserialize(std::pair<blit::Point, uint8_t> data[Board::COLS][Board::ROWS]);
  std::string to_string();

private:
  uint32_t button_debounce, update_time = 0;

  void handle_actions(blit::ButtonState &buttons);
  void handle_dpad(blit::ButtonState &buttons);
  void swap(uint8_t origin_x, uint8_t origin_y, uint8_t dest_x, uint8_t dest_y);

  uint8_t matches(std::vector<Gem*>* prev, uint8_t col, uint8_t row);
  void remove(uint8_t x, uint8_t y);
  uint8_t remove_surrounding(uint8_t col, uint8_t row);
  uint8_t remove_count(std::vector<Gem*> match_list);
};
