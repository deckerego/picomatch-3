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

void init() {
    restore_game();
}

void render(uint32_t time) {
    blit::screen.alpha = 255;
    blit::screen.mask = nullptr;
    blit::screen.pen = blit::Pen(0, 0, 0);
    blit::screen.clear();
}

void update(uint32_t time) {
  save_game();
}

void save_game() {
  SaveData data = SaveData();
  blit::write_save(data);
}

void restore_game() {
  SaveData data;
  if(blit::read_save(data)) {
  } else {
  }
}
