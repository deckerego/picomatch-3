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
#include "pico3.hpp"
#include "assets.hpp"
#include "font_asset.hpp"

using namespace blit;

const Font custom_font(chevyray);

Board board = Board();
Cursor cursor = Cursor();
TileMap* environment;
uint32_t button_debounce, last_update = 0;
uint32_t current_score, high_score = 0;
std::deque<uint8_t> bonus_scores = { };

void init() {
  set_screen_mode(ScreenMode::hires);
  screen.sprites = Surface::load(spritesheet);
  environment = new TileMap((uint8_t*)background1, nullptr, Size(32, 32), screen.sprites);
  for(int i=0; i < SCORE_SCROLL_SIZE; ++i) bonus_scores.push_front(0);
  restore_game();
}

void set_score(uint8_t matches) {
  uint32_t bonus = 0;
  if(matches > 7) bonus = matches * 25;
  if(matches > 5) bonus = matches * 10;
  if(matches > 3) bonus = matches * 5;

  bonus_scores.push_front(bonus);
  bonus_scores.pop_back();

  current_score += matches + bonus;
  if(current_score > high_score) high_score = current_score;
}

void render_cursor() {
  screen.sprite(cursor.sprite, cursor.position);
}

void render_new_scores() {
  Pen oldPen = screen.pen;

  uint8_t y_pos = 206;
  for(uint8_t bonus_score : bonus_scores) {
    y_pos -= 1;
    if(bonus_score > 3) {
      uint8_t idx = bonus_score % 20;
      uint8_t base = (idx % 6) * 15;
      if(idx <= 6) screen.pen = Pen(255 - base, base, 190);
      else if(idx <= 12) screen.pen = Pen(190, 255 - base, base);
      else screen.pen = Pen(base, 190, 255 - base);
      screen.text("+ " + std::to_string(bonus_score), custom_font, Point(48, y_pos));
    }
  }

  screen.pen = oldPen;
}

void render_score() {
  Pen oldPen = screen.pen;
  screen.pen = Pen(0xFF, 0xFF, 0xFF);
  screen.text("Score: " + std::to_string(current_score), custom_font, Point(9, 214));

  if(current_score == high_score) screen.pen = Pen(0xB2, 0xFF, 0x23);
  screen.text("Best: " + std::to_string(high_score), custom_font, Point(9, 226));

  screen.pen = oldPen;
}

void render_time() {
  Pen oldPen = screen.pen;
  uint32_t remaining_time = Board::GAME_TIME - board.time_elapsed;
  uint32_t length = (remaining_time * TIME_BAR_SIZE) / Board::GAME_TIME;

  if(remaining_time < 3000) screen.pen = Pen(0xFF, 0x5E, 0x30);
  else if(remaining_time < 7000) screen.pen = Pen(0xFF, 0xC8, 0x3D);
  else screen.pen = Pen(0xFF, 0xFF, 0xFF);

  screen.rectangle(Rect(120 + (TIME_BAR_SIZE - length), 220, length, 8));
  screen.pen = oldPen;
}

void render(uint32_t time) {
  screen.alpha = 255;
  screen.mask = nullptr;
  screen.pen = Pen(0, 0, 0);
  screen.clear();

  environment->draw(&screen, Rect(0, 0, 240, 240), nullptr);
  board.draw(screen);

  render_cursor();
  render_score();
  render_time();
  render_new_scores();
}

void update(uint32_t time) {
  board.time_elapsed += time - last_update;
  last_update = time;
  if(board.time_elapsed > Board::GAME_TIME) {
    current_score = 0;
    board.clear();
    board.initialize();
    save_game();
  }

  if(button_debounce < time) {
    bool pressed = true;
    if(buttons.state & Button::DPAD_LEFT)       cursor.move_left();
    else if(buttons.state & Button::DPAD_RIGHT) cursor.move_right();
    else if(buttons.state & Button::DPAD_DOWN)  cursor.move_down();
    else if(buttons.state & Button::DPAD_UP)    cursor.move_up();
    else pressed = false;

    if(pressed) button_debounce = time + BUTTON_DEBOUNCE_INTERVAL;
  }

  if(buttons.pressed & Button::Y) board.swap_left(cursor.location());
  if(buttons.pressed & Button::A) board.swap_right(cursor.location());
  if(buttons.pressed & Button::B) board.swap_down(cursor.location());
  if(buttons.pressed & Button::X) board.swap_up(cursor.location());

  uint8_t matches = board.mark_matches();

  set_score(matches);

  board.update();
}

void save_game() {
  SaveData data = SaveData();
  data.current_score = current_score;
  data.high_score = high_score;
  board.serialize(data.board);
  write_save(data);
}

void restore_game(bool reinitialize) {
  SaveData data;
  if(!reinitialize && read_save(data)) {
    current_score = data.current_score;
    high_score = data.high_score;
    board.deserialize(data.board);
  } else {
    current_score = 0;
    high_score = 0;
    board.initialize();
  }
}
