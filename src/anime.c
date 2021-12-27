// -*- coding: utf-8-unix -*-

#include "smb1.h"

/* colors for palette animation of coin / ?-block */
static const palette_t coin_colors[] = {
  RGB(7,4,1), RGB(6,2,0), RGB(3,0,0), RGB(6,2,0),
  RGB(7,4,1), RGB(7,4,1), RGB(7,4,1), RGB(7,4,1),
};

inline void coin_palette_animate(void) {
  /* palette animation (coin / ?-block) */
  if (!(JIFFY & 7)) {
    const uint8_t j = (JIFFY >> 3) & 7;
    vdp_set_palette(7, coin_colors[j]);
  }
}

void anime_on_vsync(void) {
  coin_palette_animate();
}

void anime_update(void) {
  if (!(tick & 1)) {
    return;
  }
  mario_animate();
}

void anime_reset_palette(void) {
  vdp_set_palette(7, coin_colors[0]);
}
