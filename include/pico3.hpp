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
#include "32blit.hpp"
#include "config.hpp"
#include "board.hpp"
#include "gem.hpp"

struct SaveData {
  uint8_t level;
  uint32_t current_score;
  uint32_t high_score;
  std::pair<blit::Point, uint8_t> board[Board::COLS][Board::ROWS];
};

void init();
void render(uint32_t time);
void update(uint32_t time);
