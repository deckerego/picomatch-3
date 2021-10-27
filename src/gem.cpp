#include "gem.hpp"

blit::Rect Gem::next_sprite() {
  return blit::Rect((sprite_index << 1) + sprite_index, 0, 3, 3);
}
