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

using namespace blit;

TileMap* environment;
Board board = Board();
Cursor cursor = Cursor();
uint32_t debounce_start = 0;
uint32_t current_score = 0;

void init() {
  set_screen_mode(ScreenMode::hires);
  screen.sprites = Surface::load(spritesheet);
  environment = new TileMap((uint8_t*)background1, nullptr, Size(32, 32), screen.sprites);
  restore_game();
}

void render_cursor() {
  screen.sprite(cursor.sprite, cursor.position);
}

void render_score() {
  Pen oldPen = screen.pen;
  screen.pen = Pen(0xFF, 0xFF, 0xFF);
  screen.text("Score: " + std::to_string(current_score), minimal_font, Point(9, 220));

  screen.pen = oldPen;
}

void render(uint32_t time) {
  screen.alpha = 255;
  screen.mask = nullptr;
  screen.pen = Pen(0, 0, 0);
  screen.clear();

  environment->draw(&screen, Rect(0, 0, 240, 240), nullptr);
  board.draw(screen);
  render_score();
  render_cursor();
}

void update(uint32_t time) {
  if(debounce_start < time) {
    bool pressed = true;
    if(buttons.state & Button::DPAD_LEFT)       cursor.move_left();
    else if(buttons.state & Button::DPAD_RIGHT) cursor.move_right();
    else if(buttons.state & Button::DPAD_DOWN)  cursor.move_down();
    else if(buttons.state & Button::DPAD_UP)    cursor.move_up();
    else pressed = false;

    if(pressed) debounce_start = time + DEBOUNCE_INTERVAL;
  }

  if(buttons.pressed & Button::Y) board.swap_left(cursor.location());
  if(buttons.pressed & Button::A) board.swap_right(cursor.location());
  if(buttons.pressed & Button::B) board.swap_down(cursor.location());
  if(buttons.pressed & Button::X) board.swap_up(cursor.location());

  current_score += board.mark_matches();
  board.update();
}

void save_game() {
  SaveData data = SaveData();
  board.serialize(data.board);
  data.current_score = current_score;
  write_save(data);
}

void restore_game() {
  SaveData data;
  if(read_save(data)) {
    current_score = data.current_score;
    board.deserialize(data.board);
  } else {
    current_score = 0;
    board.initialize();
  }
}
