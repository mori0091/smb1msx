// -*- coding: utf-8-unix -*-

#include "smb1.h"

// void anime_show_sprites(void) {
//   // Show 16 sprite characters for demo
//   struct sprite s = {0};
//   for (int i = 0; i < 32; ++i) {
//     int x =  8 + 16 * (i / 2 % 4);
//     int y = -1 + 16 * (i / 8);
//     sprite_set_xy(&s, x, y);
//     s.pat = 4 * i;
//     vmem_set_sprite(SPRITES, i, &s);
//   }
// }

void anime_clear_sprites(void) {
  // Show 16 sprite characters for demo
  struct sprite s = {.y = 217};
  for (int i = 0; i < 32; ++i) {
    vmem_set_sprite(SPRITES, i, &s);
  }
}

static void coin_palette_animate(void);
static void demo_animate(void);

void anime_update(void) {
  mario_animate();
  coin_palette_animate();
}


/* colors for palette animation of coin / ?-block */
static const palette_t coin_colors[] = {
  RGB(7,4,1), RGB(6,2,0), RGB(3,0,0), RGB(6,2,0),
  RGB(7,4,1), RGB(7,4,1), RGB(7,4,1), RGB(7,4,1),
};

static void coin_palette_animate(void) {
  /* palette animation (coin / ?-block) */
  if (!(tick & 7)) {
    const uint8_t j = (tick >> 3) & 7;
    vdp_set_palette(7, coin_colors[j]);
  }
}
