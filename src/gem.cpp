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

  if(position.x != dest_x)
    position.x += sgn(dest_x - position.x) * 2;
  if(position.y != dest_y)
    position.y += sgn(dest_y - position.y) * 2;
}

bool Gem::deletable() {
  return
    (state == Gem::VANISH || state == Gem::ASPLODE)
    && sprite_frame == 0;
}

void Gem::draw_scale(blit::Surface screen, float scale) {
  float scale_frame = sprite_frame == 0 ? scale : 1.0f + ((scale - 1.0f) / sprite_frame);
  uint32_t origin_xy = scale_frame <= 0.0f ? 0 : (Gem::SPRITE_SIZE * (scale_frame - 1.0f)) / (scale_frame * 2);
  blit::Rect sprite = blit::Rect(sprite_index * 3, sprite_row, Gem::SPRITE_SIZE / 8, Gem::SPRITE_SIZE / 8);
  blit::Point spr_orig = blit::Point(origin_xy, origin_xy);
  blit::Point spr_pos = blit::Point(position.x, position.y);
  screen.sprite(sprite, spr_pos, spr_orig, scale_frame, blit::SpriteTransform::NONE);
}

void Gem::draw_transform(blit::Surface screen, uint8_t sprite_idx, blit::SpriteTransform transformation) {
  blit::Rect sprite = blit::Rect(sprite_idx * 3, sprite_row, Gem::SPRITE_SIZE / 8, Gem::SPRITE_SIZE / 8);
  blit::Point spr_pos = blit::Point(position.x, position.y);
  screen.sprite(sprite, spr_pos, transformation);
}

void Gem::draw(blit::Surface screen) {
  if(state & Gem::VANISH) {
    draw_scale(screen, 0.0f);
  } else if(state & Gem::ASPLODE) {
    draw_scale(screen, 4.0f);
  } else if(type & Gem::SPECIAL) {
    blit::SpriteTransform transform = blit::SpriteTransform::NONE;
    uint8_t sprite_idx = sprite_frame / 10;
    if(sprite_idx == 2 || sprite_idx == 3) transform = blit::SpriteTransform::R90;
    else if(sprite_idx == 4) transform = blit::SpriteTransform::R180;
    draw_transform(screen, sprite_idx % 2, transform);
    if(sprite_frame == 0) sprite_frame = 40;
  } else {
    blit::Rect sprite = blit::Rect(sprite_index * 3, sprite_row, Gem::SPRITE_SIZE / 8, Gem::SPRITE_SIZE / 8);
    screen.sprite(sprite, position);
  }
  if(sprite_frame > 0) --sprite_frame;
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
  sprite_frame = 30;
}

void Gem::asplode() {
  state = Gem::ASPLODE;
  sprite_frame = 30;
}
