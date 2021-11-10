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
  time_elapsed = 0;
  for(uint8_t x = 0; x < Board::COLS; ++x) {
    for(uint8_t y = 0; y < Board::ROWS; ++y) {
      board[x][y] = new Gem(blit::Point(x * Gem::SPRITE_SIZE, y * Gem::SPRITE_SIZE));
    }
  }
}

void Board::draw(blit::Surface screen) {
  for(uint8_t x = 0; x < Board::COLS; ++x) {
    for(uint8_t y = 0; y < Board::ROWS; ++y) {
      board[x][y]->draw(screen);
    }
  }
}

void Board::remove(uint8_t x, uint8_t y) {
  Gem* gem = board[x][y];
  if(y > 0) {
    board[x][y] = board[x][y - 1];
    board[x][y - 1] = gem;
  } else {
    int8_t prev_y = board[x][1]->position.y;
    board[x][0] = new Gem(blit::Point(x * Gem::SPRITE_SIZE, prev_y - Gem::SPRITE_SIZE));
    delete gem;
  }
}

void Board::update() {
  for(int8_t y = Board::ROWS - 1; y >= 0; --y) {
    for(uint8_t x = 0; x < Board::COLS; ++x) {
      Gem* gem = board[x][y];
      if(gem->deletable()) remove(x, y);
      else gem->advance_to(x, y);
    }
  }
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

uint8_t remove_count(std::vector<Gem*> match_list) {
  uint8_t matched = 0;
  uint8_t list_size = match_list.size();

  if(list_size > 3) {
    for(Gem* gem : match_list) gem->asplode();
    matched += list_size;
  } else if(list_size > 2) {
    for(Gem* gem : match_list) gem->vanish();
    matched += list_size;
  }

  return matched;
}

uint8_t matches(std::vector<Gem*>* prev, Gem* current) {
  uint8_t matched = 0;

  if(prev->back()->sprite_index != current->sprite_index) {
    matched += remove_count(*prev);
    prev->clear();
  }
  prev->push_back(current);

  return matched;
}

uint8_t Board::mark_matches() {
  uint8_t matched = 0;
  std::vector<Gem*> prev_x, prev_y;

  //Address things as a 9x9 square
  for(uint8_t y = 0; y < Board::ROWS; ++y) {
    if(!board[0][y]->eligible() || !board[y][0]->eligible())
      return matched; //Quit early if we aren't ready

    prev_x = { board[0][y] };
    prev_y = { board[y][0] };

    for(uint8_t x = 1; x < Board::COLS; ++x) {
      if(!board[x][y]->eligible()) return matched;
      matched += matches(&prev_x, board[x][y]);

      if(x < Board::ROWS) {
        if(!board[y][x]->eligible()) return matched;
        matched += matches(&prev_y, board[y][x]);
      }
    }

    matched += remove_count(prev_x);
    matched += remove_count(prev_y);
  }

  //Take care of the last columns in a wide rectangular board
  for(uint8_t x = Board::ROWS; x < Board::COLS; ++x) {
    if(!board[x][0]->eligible()) return matched;
    prev_y = { board[x][0] };

    for(uint8_t y = 1; y < Board::ROWS; ++y) {
      if(!board[x][y]->eligible()) return matched;
      matched += matches(&prev_y, board[x][y]);
    }

    matched += remove_count(prev_y);
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
