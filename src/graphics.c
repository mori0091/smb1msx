// -*- coding: utf-8-unix -*-

#include <msx.h>

#include "graphics.h"
#include "macros.h"

void graphics_init_vdp(void) {
  /* Set backdrop color (border color of the screen) */
  vdp_set_color(0x00);

  vdp_set_visible(false);
  vdp_set_screen_mode(SCREEN_MODE);
  vdp_set_screen_lines(SCREEN_LINES);

  // vdp_set_pattern_table(PATTERNS);
  // vdp_set_color_table(COLORS);

  vdp_set_sprite_visible(false);
  vdp_set_sprite_attribute_table(SPRITES);
  vdp_set_sprite_pattern_table(SPRITE_PATTERNS);
  vdp_set_sprite_size(VDP_SPRITE_SIZE_16x16);

  /* Dual page horizontal scroll mode */
  vdp_set_image_table(IMAGE + SIZE_OF_VRAM_PAGE); /* SET PAGE 1 */
  vdp_set_hscroll_dual_page(true); /* enable 2 pages horizontal scroll */
  vdp_set_hscroll_mask(true);      /* hide ugly left border of 8 pixels */
  vdp_set_adjust(-4, 0);           /* centering visible display area */
}

#define VRAM_PAGE_FILL(pp, byte)                              \
  vdp_cmd_execute_HMMV(0,                                     \
                       (pp)*LINES_PER_VRAM_PAGE,              \
                       PIXELS_PER_LINE,                       \
                       LINES_PER_VRAM_PAGE,                   \
                       (byte))

void graphics_clear_vram(void) {
  /* clear VRAM page #0 and #1 */
  VRAM_PAGE_FILL(0, 0x00);
  VRAM_PAGE_FILL(1, 0x00);

  // In rare cases, access to VRAM may fail or may be omitted depending on the
  // timing of the access. This occurs during a VDP command is executing.
  // Therefore, waits for VDP command finished before accsess to VRAM.
  vdp_cmd_await();
  /* Clear sprites */
  graphics_hide_all_sprites();
}

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
