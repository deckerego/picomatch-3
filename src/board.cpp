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
#include "board.hpp"

void Board::initialize() {
  for(uint8_t x = 0; x < Board::COLS; ++x) {
    for(uint8_t y = 0; y < Board::ROWS; ++y) {
      board[x][y] = new Gem(blit::Point(x * Gem::SPRITE_SIZE, y * Gem::SPRITE_SIZE));
    }
  }
}

Gem* Board::get(uint8_t x, uint8_t y) {
  return board[x][y];
}

void Board::swap_left(std::pair<uint8_t, uint8_t> location) {
 swap(location.first, location.second, location.first - 1, location.second);
}

void Board::swap_right(std::pair<uint8_t, uint8_t> location) {
 swap(location.first, location.second, location.first + 1, location.second);
}

void Board::swap_up(std::pair<uint8_t, uint8_t> location) {
 swap(location.first, location.second, location.first, location.second - 1);
}

void Board::swap_down(std::pair<uint8_t, uint8_t> location) {
 swap(location.first, location.second, location.first, location.second + 1);
}

void Board::swap(uint8_t origin_x, uint8_t origin_y, uint8_t dest_x, uint8_t dest_y) {
  if(dest_x >= Board::COLS || dest_y >= Board::ROWS) return;
  Gem* swap = board[dest_x][dest_y];
  board[dest_x][dest_y] = board[origin_x][origin_y];
  board[origin_x][origin_y] = swap;
}

uint8_t Board::mark_matches_horiz(uint8_t x, uint8_t y) {
  Gem* i = board[x][y];
  std::vector<uint8_t> match_horiz = {x};

  for(uint8_t h = x + 1; h < Board::COLS; ++h) {
    if(i->sprite_index == board[h][y]->sprite_index) match_horiz.push_back(h);
    else break;
  }

  uint8_t matched = match_horiz.size();
  return matched > 2 ? matched : 0;
}

uint8_t Board::mark_matches_vert(uint8_t x, uint8_t y) {
  Gem* i = board[x][y];
  std::vector<uint8_t> match_vert = {y};

  for(uint8_t v = y + 1; v < Board::ROWS; ++v) {
    if(i->sprite_index == board[x][v]->sprite_index) match_vert.push_back(v);
    else break;
  }

  uint8_t matched = match_vert.size();
  return matched > 2 ? matched : 0;
}

uint8_t Board::mark_matches() {
  uint8_t matched = 0;

  for(uint8_t y = 0; y < Board::ROWS; ++y) {
    for(uint8_t x = 0; x < Board::COLS; ++x) {
      Gem* root = board[x][y];
      if(root == nullptr) break;
      matched += mark_matches_horiz(x, y);
      matched += mark_matches_vert(x, y);
    }
  }

  return matched;
}

void Board::serialize(std::pair<blit::Point, uint8_t> data[Board::COLS][Board::ROWS]) {
  for(uint8_t y = 0; y < Board::ROWS; ++y) {
    for(uint8_t x = 0; x < Board::COLS; ++x) {
      data[x][y] = { board[x][y]->position, board[x][y]->sprite_index };
    }
  }
}

void Board::deserialize(std::pair<blit::Point, uint8_t> data[Board::COLS][Board::ROWS]) {
  for(uint8_t y = 0; y < Board::ROWS; ++y) {
    for(uint8_t x = 0; x < Board::COLS; ++x) {
      board[x][y] = new Gem(data[x][y].first, data[x][y].second);
    }
  }
}
