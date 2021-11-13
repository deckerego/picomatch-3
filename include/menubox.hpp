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

struct MenuItem {
  void (*callback)();
  std::string label;

  MenuItem(std::string label, void (*callback)()) : callback(callback), label(label) {};
};

struct MenuBox {
  static const uint8_t ACTIVE   = 0;
  static const uint8_t INACTIVE = 1;

  uint8_t state = MenuBox::INACTIVE;
  uint8_t selected = 0;
  std::vector<MenuItem> items = { MenuItem("Continue", nullptr) };

  MenuBox() : state(MenuBox::INACTIVE) {};

  void add_item(std::string text, void (*callback)());
  void draw(blit::Surface screen);
  void press(blit::ButtonState &buttons);
  void update(uint32_t time);

private:
  uint32_t button_debounce, update_time = 0;

  void handle_dpad(blit::ButtonState &buttons);
  void handle_actions(blit::ButtonState &buttons);
  void close();
};
