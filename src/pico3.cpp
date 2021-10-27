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

void init() {
  set_screen_mode(ScreenMode::hires);
  screen.sprites = Surface::load(spritesheet);
  environment = new TileMap((uint8_t*)background1, nullptr, Size(32, 32), screen.sprites);
  restore_game();
}

void render(uint32_t time) {
  screen.alpha = 255;
  screen.mask = nullptr;
  screen.pen = Pen(0, 0, 0);
  screen.clear();

  environment->draw(&screen, Rect(0, 0, 240, 240), nullptr);
  for(uint8_t x = 0; x < 10; ++x) {
    for(uint8_t y = 0; y < 9; ++y) {
      Rect sprite = Rect(((x * y) % 5) * 3, 0, 3, 3);
      Point position = Point(x * 24, y * 24);
      screen.sprite(sprite, position);
    }
  }
}

void update(uint32_t time) {
  save_game();
}

void save_game() {
  SaveData data = SaveData();
  write_save(data);
}

void restore_game() {
  SaveData data;
  if(read_save(data)) {
  } else {
  }
}
