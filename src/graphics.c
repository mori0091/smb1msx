// -*- coding: utf-8-unix -*-

#include <msx.h>

#include "graphics.h"
#include "macros.h"

// static const struct sprite hidden_sprite = { .y = 217 };

// void graphics_hide_sprite(uint8_t plane) {
//   vmem_set_sprite(SPRITES_0, plane, &hidden_sprite);
// }
void graphics_hide_sprite(uint8_t plane) {
  vmem_set_write_address(SPRITES_0 + sizeof(struct sprite) * plane + 0);
  vmem_set(217);
}

void graphics_hide_all_sprites(void) {
  for (uint8_t i = 0; i < 32; ++i) {
    graphics_hide_sprite(i);
  }
}

void graphics_fill_sprite_pat(uint8_t pat, uint8_t x, uint8_t n_bytes) {
  vmem_memset(SPRITE_PATTERNS + 8 * pat, x, n_bytes);
}

void graphics_set_sprite_pat(uint8_t pat, const uint8_t* p, uint8_t n_bytes) {
  vmem_write(SPRITE_PATTERNS + 8 * pat, p, n_bytes);
}

void graphics_set_sprite(uint8_t plane, const struct sprite* s) {
  vmem_set_sprite(SPRITES_0, plane, s);
}
