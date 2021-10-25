// -*- coding: utf-8-unix -*-

#include "smb1.h"

void anime_show_sprites(void) {
  // Show 16 sprite characters for demo
  struct sprite s = {0};
  for (int i = 0; i < 32; ++i) {
    int x =  8 + 16 * (i / 2 % 4);
    int y = -1 + 16 * (i / 8);
    sprite_set_xy(&s, x, y);
    s.pat = 4 * i;
    vmem_write(SPRITES + 4 * i, &s, sizeof(struct sprite));
  }
  // ---- Mario (player) ---- (#0,#1)
  sprite_set_xy(&s, 124, 175);
  s.pat = 0;
  vmem_write(SPRITES + 0, &s, sizeof(struct sprite));
  s.pat = 4;
  vmem_write(SPRITES + 4, &s, sizeof(struct sprite));
}

static void coin_palette_animate(void);
static void demo_animate(void);

void anime_update(void) {
  mario_animate();
  coin_palette_animate();
  // ---------------------------------------
  demo_animate();               /* for debug purpose */
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

static void demo_animate(void) {
  const uint8_t p = WALKING_1 + mario_state.facing;
  uint8_t j;
  // sprite animation #1 (every 1 frame ; 60fps @ 60Hz)
  {
    j = tick % 3;
    vmem_write(SPRITE_PATTERNS+0x100, smb1spt + 64 * (2*j + p), 64);
  }
  // sprite animation #2 (every 2 frames ; 30fps @ 60Hz)
  if (!(tick & 1 ^ 1)) {
    j = (tick >> 1) % 3;
    vmem_write(SPRITE_PATTERNS+0x140, smb1spt + 64 * (2*j + p), 64);
  }
  // sprite animation #3 (every 4 frames ; 15fps @ 60Hz)
  if (!(tick & 3 ^ 2)) {
    j = (tick >> 2) % 3;
    vmem_write(SPRITE_PATTERNS+0x180, smb1spt + 64 * (2*j + p), 64);
  }
  // sprite animation #4 (every 8 frames ; 7.5fps @ 60Hz)
  if (!(tick & 7 ^ 4)) {
    j = (tick >> 3) % 3;
    vmem_write(SPRITE_PATTERNS+0x1C0, smb1spt + 64 * (2*j + p), 64);
  }
}
