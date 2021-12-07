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
  update_time = 0;
  state = Board::NONE;

  for(uint8_t x = 0; x < Board::COLS; ++x) {
    for(uint8_t y = 0; y < Board::ROWS; ++y) {
      int32_t y_pos = ((y - Board::ROWS) * Gem::SPRITE_SIZE) + (8 * y);
      int32_t x_pos = x * Gem::SPRITE_SIZE;
      board[x][y] = new Gem(blit::Point(x_pos, y_pos), gem_type);
    }
  }
}

void Board::clear() {
  state = Board::CLEAR;
  for(uint8_t x = 0; x < Board::COLS; ++x) {
    for(uint8_t y = 0; y < Board::ROWS; ++y) {
      board[x][y]->asplode();
    }
  }
}

bool Board::cleared() {
  return
    state == Board::CLEAR &&
    board[Board::COLS - 1][Board::ROWS - 1] == nullptr;
}

void Board::draw(blit::Surface screen) {
  for(uint8_t x = 0; x < Board::COLS; ++x) {
    for(uint8_t y = 0; y < Board::ROWS; ++y) {
      if(board[x][y] != nullptr) board[x][y]->draw(screen);
    }
  }

  screen.sprite(cursor.sprite, cursor.position);
}

void Board::remove(uint8_t x, uint8_t y) {
  Gem* gem = board[x][y];
  if(y > 0) {
    board[x][y] = board[x][y - 1];
    board[x][y - 1] = gem;
  } else {
    int8_t prev_y = board[x][1]->position.y;
    board[x][0] = state == Board::CLEAR ? nullptr : new Gem(blit::Point(x * Gem::SPRITE_SIZE, prev_y - Gem::SPRITE_SIZE), gem_type);
    delete gem;
  }
}

void Board::swap(uint8_t origin_x, uint8_t origin_y, uint8_t dest_x, uint8_t dest_y) {
  if(dest_x >= Board::COLS || dest_y >= Board::ROWS) return;

  Gem* swap = board[origin_x][origin_y];
  board[origin_x][origin_y] = board[dest_x][dest_y];
  board[dest_x][dest_y] = swap;

  if(swap->state & Gem::PRIMED) swap->state ^= Gem::PRIMED;
}

void Board::handle_dpad(blit::ButtonState &buttons) {
  if(button_debounce < update_time) {
    bool pressed = true;
    if(buttons.state & blit::Button::DPAD_LEFT)       cursor.move_left();
    else if(buttons.state & blit::Button::DPAD_RIGHT) cursor.move_right();
    else if(buttons.state & blit::Button::DPAD_DOWN)  cursor.move_down();
    else if(buttons.state & blit::Button::DPAD_UP)    cursor.move_up();
    else pressed = false;

    if(pressed) button_debounce = update_time + BUTTON_DEBOUNCE_INTERVAL;
  }
}

void Board::handle_actions(blit::ButtonState &buttons) {
  std::pair<uint8_t, uint8_t> location = cursor.location();
  if(buttons.pressed & blit::Button::Y)
    swap(location.first, location.second, location.first - 1, location.second);
  if(buttons.pressed & blit::Button::A)
    swap(location.first, location.second, location.first + 1, location.second);
  if(buttons.pressed & blit::Button::X)
    swap(location.first, location.second, location.first, location.second - 1);
  if(buttons.pressed & blit::Button::B)
    swap(location.first, location.second, location.first, location.second + 1);
}

void Board::press(blit::ButtonState &buttons) {
  handle_dpad(buttons);
  handle_actions(buttons);
}

void Board::update(uint32_t time) {
  if(update_time > 0) time_elapsed += time - update_time;
  update_time = time;

  for(int8_t y = Board::ROWS - 1; y >= 0; --y) {
    for(uint8_t x = 0; x < Board::COLS; ++x) {
      Gem* gem = board[x][y];
      if(gem->deletable()) remove(x, y);
      else gem->advance_to(x, y);
    }
  }
}

// Eliminated all surrounding gems
uint8_t Board::remove_surrounding(uint8_t col, uint8_t row) {
  uint8_t matched = 0;
  for(int8_t h = -1; h <= 1; ++h) {
    int8_t x = col + h;
    if(x < 0 || x >= Board::COLS) continue;

    for(int8_t v = -1; v <= 1; ++v) {
      int8_t y = row + v;
      if(y < 0 || y >= Board::ROWS) continue;

      board[x][y]->vanish();
      matched += 2;
    }
  }
  board[col][row]->asplode();
  return matched;
}

// Remove all gems in a row
uint8_t Board::remove_count(std::vector<Gem*> match_list) {
  uint8_t matched = 0;
  uint8_t list_size = match_list.size();

  if(list_size > 4) {
    match_list.back()->special();
    match_list.pop_back();
    for(Gem* gem : match_list) gem->asplode();
    matched += list_size;
  } else if(list_size > 3) {
    for(Gem* gem : match_list) gem->asplode();
    matched += list_size;
  } else if(list_size > 2) {
    for(Gem* gem : match_list) gem->vanish();
    matched += list_size;
  }

  return matched;
}

uint8_t Board::matches(std::vector<Gem*>* prev, uint8_t col, uint8_t row) {
  Gem* current = board[col][row];
  uint8_t matched = 0;

  if(current->type == Gem::SPECIAL) {
    matched += remove_surrounding(col, row);
    prev->clear();
  } else if(!prev->back()->equals(current)) {
    matched += remove_count(*prev);
    prev->clear();
  }
  prev->push_back(current);

  return matched;
}

uint8_t Board::mark_matches() {
  if(state != Board::NONE) return 0;

  uint8_t matched = 0;
  std::vector<Gem*> prev_x, prev_y;

  //Address things as a 9x9 square
  for(uint8_t y = 0; y < Board::ROWS; ++y) {
    if(!board[0][y]->eligible() || !board[y][0]->eligible())
      return matched; //Quit early if we aren't ready

    prev_x = { board[0][y] };
    prev_y = { board[y][0] };

    for(uint8_t x = 1; x < Board::COLS; ++x) {
      if(board[x][y]->eligible()) {
        matched += matches(&prev_x, x, y);
      }

      if(x < Board::ROWS) {
        if(board[y][x]->eligible()) {
          matched += matches(&prev_y, y, x);
        }
      }
    }

    matched += remove_count(prev_x);
    matched += remove_count(prev_y);
  }

  //Take care of the last columns in a wide rectangular board
  for(uint8_t x = Board::ROWS; x < Board::COLS; ++x) {
    if(board[x][0]->eligible()) {
      prev_y = { board[x][0] };
    }

    for(uint8_t y = 1; y < Board::ROWS; ++y) {
      if(board[x][y]->eligible()) {
        matched += matches(&prev_y, x, y);
      }
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
      board[x][y] = new Gem(data[x][y].first, gem_type, data[x][y].second);
    }
  }
}

std::string Board::to_string() {
  std::string board_str = "";
  for(uint8_t y = 0; y < Board::ROWS; ++y) {
    board_str += "| ";
    for(uint8_t x = 0; x < Board::COLS; ++x) {
      board_str += "[" + board[x][y]->to_string() + "] ";
    }
    board_str += "|\n";
  }
  return board_str;
}
