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
#include "board.hpp"
#include "gem.hpp"
#include "cursor.hpp"

const uint16_t BUTTON_DEBOUNCE_INTERVAL = 200;
const uint16_t SAVE_DEBOUNCE_INTERVAL   = 2000;
const uint8_t  SCORE_SCROLL_SIZE        = 160;
const uint8_t  TIME_BAR_SIZE            = 116;

struct SaveData {
  uint32_t current_score;
  uint32_t high_score;
  std::pair<blit::Point, uint8_t> board[Board::COLS][Board::ROWS];
};

void init();

void render(uint32_t time);
void render_cursor();
void render_board();

void update(uint32_t time);

void save_game();
void restore_game(bool reinitialize=false);
