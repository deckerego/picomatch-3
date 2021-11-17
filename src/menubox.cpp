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

#include "menubox.hpp"

using namespace blit;

void MenuBox::add_item(std::string label, void (*callback)()) {
  items.push_back(MenuItem(label, callback));
}

void MenuBox::handle_dpad(ButtonState &buttons) {
  if(button_debounce < update_time) {
    bool pressed = true;
    if(buttons.state & Button::DPAD_DOWN)    selected = (selected + 1) % items.size();
    else if(buttons.state & Button::DPAD_UP) selected = (selected - 1) % items.size();
    else pressed = false;

    if(pressed) button_debounce = update_time + BUTTON_DEBOUNCE_INTERVAL;
  }
}

void MenuBox::handle_actions(ButtonState &buttons) {
  if(buttons.pressed & Button::A) {
    MenuItem item = items.at(selected);
    if(item.callback != nullptr) item.callback();
    state = MenuBox::INACTIVE;
  } else if(buttons.pressed & Button::B) {
    state = MenuBox::INACTIVE;
  }
}

void MenuBox::press(ButtonState &buttons) {
  if(state == MenuBox::INACTIVE) return;
  handle_dpad(buttons);
  handle_actions(buttons);
}

void MenuBox::update(uint32_t time) {
  if(state == MenuBox::INACTIVE) return;
  update_time = time;
}

void MenuBox::draw(Surface screen) {
  if(state == MenuBox::INACTIVE) return;

  Pen oldPen = screen.pen;

  screen.pen = Pen(0x00, 0x00, 0x00);
  screen.rectangle(Rect(36, 36, 168, 96));

  screen.pen = Pen(0xFF, 0x66, 0xFF);
  screen.text("Pico", default_font, Point(52, 52));
  screen.pen = Pen(0x66, 0xFF, 0xFF);
  screen.text("Match", default_font, Point(82, 52));
  screen.pen = Pen(0xFF, 0xFF, 0x66);
  screen.text("3", default_font, Point(124, 52));

  screen.pen = Pen(0xFF, 0xFF, 0xFF);
  uint32_t x = 60, y = 72;
  for(MenuItem item : items) {
    screen.text(item.label, default_font, Point(x, y));
    y += 20;
  }
  screen.text(">", default_font, Point(52, (selected * 20) + 72));

  screen.pen = oldPen;
};
