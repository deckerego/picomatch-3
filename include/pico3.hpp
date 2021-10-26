#include <cstdint>
#include "32blit.hpp"

struct SaveData {
};

void init();

void render(uint32_t time);

void update(uint32_t time);

void save_game();
void restore_game();
