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

#ifndef LIB_PICOMATCH_CONFIG
#define LIB_PICOMATCH_CONFIG

#include "32blit.hpp"
#include "font_asset.hpp"

const blit::Font default_font(thicket_12);
const blit::Font header_font(picory_smooth_24);
const uint32_t   GAME_TIME                = 4 * 1000;
const uint16_t   BUTTON_DEBOUNCE_INTERVAL = 200;
const uint16_t   SAVE_DEBOUNCE_INTERVAL   = 2000;
const uint8_t    SCORE_SCROLL_SIZE        = 160;
const uint8_t    TIME_BAR_SIZE            = 116;

#endif
