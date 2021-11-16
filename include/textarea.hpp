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

struct TextItem {
  static const uint8_t NONE   = 0;
  static const uint8_t ZOOM   = 1;
  static const uint8_t LEFT   = 2;
  static const uint8_t UP     = 4;
  static const uint8_t RCOLOR = 8;

  std::string label;
  blit::Point position;
  uint8_t effects = NONE;
  uint8_t frame = 200;

  TextItem(std::string label, blit::Point pos, uint8_t effect) : label(label), position(pos), effects(effect) {};
};

struct TextArea {
  static const uint8_t ACTIVE   = 0;
  static const uint8_t INACTIVE = 1;

  uint8_t state = TextArea::INACTIVE;
  std::vector<TextItem*> items = { };

  TextArea() : state(TextArea::INACTIVE) {};

  void draw(blit::Surface screen);
  void update(uint32_t time);
  void add_item(std::string text, uint32_t x, uint32_t y, uint8_t effects);

private:
  uint32_t update_time = 0;
};
