#include <cstdint>
#include "pico3.hpp"
#include "assets.hpp"

void init() {
    restore_game();
}

void render(uint32_t time) {
    screen.alpha = 255;
    screen.mask = nullptr;
    screen.pen = Pen(0, 0, 0);
    screen.clear();
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
