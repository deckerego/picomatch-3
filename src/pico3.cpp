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
#include "menubox.hpp"
#include "textarea.hpp"
#include "assets.hpp"

using namespace blit;

Board board = Board();
MenuBox menu = MenuBox();
TextArea textarea = TextArea();
TileMap* environment;
uint8_t level = 0;
uint32_t current_score, high_score = 0;

uint8_t* get_background(uint8_t idx) {
  if(idx == 1) return (uint8_t *)background2;
  if(idx == 2) return (uint8_t *)background3;
  if(idx == 3) return (uint8_t *)background4;
  return (uint8_t *)background1;
}

void set_score(uint8_t matches) {
  uint32_t bonus = 0;
  if(matches > 7) bonus = matches * 25;
  if(matches > 5) bonus = matches * 10;
  if(matches > 3) bonus = matches * 5;

  if(bonus > 0)
    textarea.add_item("+ " + std::to_string(bonus), 48, 206, TextItem::RCOLOR | TextItem::ZOOM | TextItem::UP);

  current_score += matches + bonus;
  if(current_score > high_score) high_score = current_score;
}

void save_game() {
  SaveData data = SaveData();
  data.level = level;
  data.current_score = current_score;
  data.high_score = high_score;
  board.serialize(data.board);
  write_save(data);
}

void restore_game(bool reinitialize = false) {
  SaveData data;
  if(!reinitialize && read_save(data)) {
    level = data.level;
    current_score = data.current_score;
    high_score = data.high_score;
    board.deserialize(data.board);
  } else {
    level = 0;
    current_score = 0;
    high_score = 0;
    board.initialize();
  }
}

void next_level() {
  uint8_t background = ++level % 4;
  environment = new TileMap(get_background(background), nullptr, Size(32, 32), screen.sprites);
  current_score = 0;
  board.initialize();
  save_game();
  textarea.add_item("Level", 60, 220, TextItem::ZOOM | TextItem::UP);
  textarea.add_item(std::to_string(level), 220, 120, TextItem::ZOOM | TextItem::LEFT);
}

void render_score() {
  Pen oldPen = screen.pen;
  screen.pen = Pen(0xFF, 0xFF, 0xFF);
  screen.text("Score: " + std::to_string(current_score), default_font, Point(9, 214));

  if(current_score == high_score) screen.pen = Pen(0xB2, 0xFF, 0x23);
  screen.text("Best: " + std::to_string(high_score), default_font, Point(9, 226));

  screen.pen = oldPen;
}

void render_time() {
  Pen oldPen = screen.pen;
  int32_t remaining_time = GAME_TIME - board.time_elapsed;
  uint32_t length = remaining_time > 0 ? (remaining_time * TIME_BAR_SIZE) / GAME_TIME : 0;

  if(remaining_time < 3000) screen.pen = Pen(0xFF, 0x5E, 0x30);
  else if(remaining_time < 7000) screen.pen = Pen(0xFF, 0xC8, 0x3D);
  else screen.pen = Pen(0xFF, 0xFF, 0xFF);

  screen.rectangle(Rect(120 + (TIME_BAR_SIZE - length), 220, length, 8));
  screen.pen = oldPen;
}

void reset() {
  restore_game(true);
}

void init() {
  restore_game();

  menu.state = MenuBox::ACTIVE;
  menu.add_item("Reset Scores", &reset);

  set_screen_mode(ScreenMode::hires);
  screen.sprites = Surface::load(spritesheet);

  environment = new TileMap(get_background(level), nullptr, Size(32, 32), screen.sprites);
}

void render(uint32_t time) {
  screen.alpha = 255;
  screen.mask = nullptr;
  screen.pen = Pen(0, 0, 0);
  screen.clear();

  environment->draw(&screen, Rect(0, 0, 240, 240), nullptr);

  if(menu.state == MenuBox::ACTIVE) {
    menu.draw(screen);
  } else {
    board.draw(screen);
  }

  textarea.draw(screen);
  render_score();
  render_time();
}

void update(uint32_t time) {
  if(menu.state == MenuBox::ACTIVE) {
    menu.press(buttons);
    menu.update(time);
  } else if(board.time_elapsed > GAME_TIME) {
    board.update(time);
    if(board.state != Board::CLEAR) board.clear();
    if(board.cleared()) next_level();
  } else {
    board.press(buttons);
    set_score(board.mark_matches());
    board.update(time);
  }

  textarea.update(time);
}
