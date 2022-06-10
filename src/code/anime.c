// -*- coding: utf-8-unix -*-

#include <vdp.h>

#include "anime.h"
#include "assert.h"
#include "graphics.h"
#include "macros.h"
#include "mario.h"
#include "sm2.h"
#include "timer.h"

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

static const uint8_t invincible_colors[][3] = {
  { 1, 2, 3, },
  { 13, 15, 14, },
  { 5, 15, 14, },
  { 9, 11, 10, },
};

static void invincible_palette_animate(void) {
  const uint8_t i = (JIFFY >> 1) & 3;
  vdp_set_palette(1, color_palette[invincible_colors[i][0]]);
  vdp_set_palette(2, color_palette[invincible_colors[i][1]]);
  vdp_set_palette(3, color_palette[invincible_colors[i][2]]);
}

// static const uint8_t weakened_colors[][3] = {
//   { 1, 2, 3, },
//   { 12, 12, 12, },
// };

// static void weakened_palette_animate(void) {
//   const uint8_t i = JIFFY & 1;
//   vdp_set_palette(1, color_palette[weakened_colors[i][0]]);
//   vdp_set_palette(2, color_palette[weakened_colors[i][1]]);
//   vdp_set_palette(3, color_palette[weakened_colors[i][2]]);
// }

static volatile bool enable_on_vsync;

void anime_set_enable_on_vsync(bool enable) {
  __critical {
    enable_on_vsync = enable;
    if (!enable) {
      vdp_write_palette(color_palette);
      // vdp_set_sprite_attribute_table(SPRITES);
    }
  }
}

void anime_on_vsync(void) {
  if (!enable_on_vsync) {
    return;
  }

  // palette animations
  {
    coin_palette_animate();
  }
  if (mario_is_invincible()) {
    // multi-color blinking
    invincible_palette_animate();
  }
  if (mario_is_weakened()) {
    // blinking (pseudo transparent effect)
    if (!sm2_is_dirty()) {
      // hide sprite plane #2
      graphics_hide_sprite(2);
    }
  }
  if (mario_state.status_timer) {
    if (!(--mario_state.status_timer)) {
      mario_reset_physical_status();
    }
  }
}

void anime_update(void) {
  mario_animate();
}
