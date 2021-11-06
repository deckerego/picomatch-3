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

#include "gem.hpp"

void Gem::advance_to(uint8_t x, uint8_t y) {
  uint8_t dest_x = Gem::SPRITE_SIZE * x;
  uint8_t dest_y = Gem::SPRITE_SIZE * y;

  position.x += sgn(dest_x - position.x) * 2;
  position.y += sgn(dest_y - position.y) * 2;

  if(sprite_frame > 0) --sprite_frame;
}

bool Gem::deletable() {
  return
    (state == Gem::VANISH || state == Gem::ASPLODE)
    && sprite_frame == 0;
}

void Gem::draw(blit::Surface screen) {
  blit::Rect sprite = blit::Rect(sprite_index * 3, 0, Gem::SPRITE_SIZE / 8, Gem::SPRITE_SIZE / 8);
  if(state == Gem::VANISH) {
    float scale = sprite_frame == 0 ? 0.0f : 1.0f - (1.0f / sprite_frame);
    uint32_t origin_xy = (Gem::SPRITE_SIZE * scale) - Gem::SPRITE_SIZE;
    blit::Point spr_orig = blit::Point(origin_xy, origin_xy);
    blit::Point spr_pos = blit::Point(position.x, position.y);
    screen.sprite(sprite, spr_pos, spr_orig, scale, blit::SpriteTransform::NONE);
  } else if(state == Gem::ASPLODE) {
    float scale = sprite_frame == 0 ? 8.0f : 1.0f + (7.0f / sprite_frame);
    uint32_t origin_xy = (Gem::SPRITE_SIZE / (scale * 2)) * (scale - 1.0f);
    blit::Point spr_orig = blit::Point(origin_xy, origin_xy);
    blit::Point spr_pos = blit::Point(position.x, position.y);
    screen.sprite(sprite, spr_pos, spr_orig, scale, blit::SpriteTransform::NONE);
  } else {
    screen.sprite(sprite, position);
  }
}

bool Gem::left_of(Gem* of) {
  return position.y == of->position.y && (position.x + Gem::SPRITE_SIZE) == of->position.x;
}

bool Gem::up_of(Gem* of) {
  return position.x == of->position.x && (position.y + Gem::SPRITE_SIZE) == of->position.y;
}

bool Gem::eligible() {
  return state == Gem::NONE && position.y >= 0;
}

void Gem::vanish() {
  state = Gem::VANISH;
  sprite_frame = 20;
}

void Gem::asplode() {
  state = Gem::ASPLODE;
  sprite_frame = 20;
}
