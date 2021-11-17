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

#include "textarea.hpp"

using namespace blit;

void TextArea::add_item(std::string label, int32_t x, int32_t y, uint8_t effects) {
  Point position = Point(x, y);
  items.push_back(new TextItem(label, position, effects));
  state = TextArea::ACTIVE;
}

Pen color_random() {
  uint8_t idx = blit::random() % 20;
  uint8_t base = (idx % 6) * 15;
  if(idx <= 6) return Pen(255 - base, base, 190);
  if(idx <= 12) return Pen(190, 255 - base, base);
  return Pen(base, 190, 255 - base);
}

Pen get_color(TextItem* item) {
  if(item->effects & TextItem::RCOLOR) return color_random();
  return Pen(0xFF, 0xFF, 0xFF);
}

Font get_font(TextItem* item) {
  if(item->effects & TextItem::HEADER) return header_font;
  return default_font;
}

void TextArea::update(uint32_t time) {
  if(state == TextArea::INACTIVE) return;

  update_time = time;
  std::vector<TextItem*> items_new = { };

  for(TextItem* item : items) {
    if(item->effects & TextItem::ZOOM) {
      if(item->effects & TextItem::UP) item->position.y -= 1;
      if(item->effects & TextItem::LEFT) item->position.x -= 1;
    }

    if(item->position.x < 0 || item->position.x > 240 ||
       item->position.y < 0 || item->position.y > 240) {
      delete item;
    } else {
      items_new.push_back(item);
    }
  }
  items = items_new;

  if(items.size() == 0) {
    state = TextArea::INACTIVE;
  }
}

void TextArea::draw(Surface screen) {
  if(state == TextArea::INACTIVE) return;

  Pen oldPen = screen.pen;

  for(TextItem* item : items) {
    screen.pen = get_color(item);
    screen.text(item->label, get_font(item), item->position);
  }

  screen.pen = oldPen;
};
