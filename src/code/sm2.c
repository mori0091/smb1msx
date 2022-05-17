// -*- coding: utf-8-unix -*-

#include "../bank0/smb1.h"
#include <stdint.h>

static void sprites_from_metasprite(struct sprite * sp,
                                    const metasprite_t * ms,
                                    int x, int y) {
  const uint8_t n = ms->n;
  const int dx = x - ms->anchor.x;
  const int dy = y - ms->anchor.y;
  for (uint8_t i = 0; i < n; ++i) {
    sp->pat = ms->pats[i];
    sprite_set_x(sp, ms->layouts[i].x + dx);
    sprite_set_y(sp, ms->layouts[i].y + dy);
    if (sp->tagged_color & SPRITE_TAG_EC) {
      // if there is a sprite whose x-coordinate would to be less than 0 on the
      // screen, that sprite is hidden instead of setting the EC bit.
      sp->y = 217;
    }
    sp++;
  }
}

static struct sprite sprites[32];

// dirty range [beg, end)
static uint8_t beg;
static uint8_t end;

static void on_modified(uint8_t plane, uint8_t n) {
  uint8_t last = plane + n;
  if (end < last) {
    end = last;
  }
  if (plane < beg) {
    beg = plane;
  }
}

void sm2_init(void) {
  struct sprite * sp = sprites;
  uint8_t n = 32;
  while (n--) {
    sp->y = 217;
    sp++;
  }
}

void sm2_clear_sprites(void) {
  beg = 255;
  end = 0;
  sm2_init();
}

void sm2_hide_sprites(uint8_t plane, const metasprite_t * ms) {
  struct sprite * sp = sprites + plane;
  uint8_t n = ms->n;
  while (n--) {
    sp->y = 217;
    sp++;
  }
  on_modified(plane, ms->n);
}

void sm2_show_sprites(uint8_t plane, const metasprite_t * ms, int x, int y) {
  // assert(ms && plane + ms->n <= 32);
  sprites_from_metasprite(sprites + plane, ms, x, y);
  on_modified(plane, ms->n);
}

// void sm2_apply_sprites(void) {
//   if (sm2_is_dirty()) {
//     // \note Don't change [beg, end) range.
//     uint8_t endIdx = end;
//     if (endIdx < sizeof(sprites)/sizeof(sprites[0])) {
//       // Hides all the sprites whose plane number is greater than or equal to
//       // endIdx.
//       sprites[endIdx++].y = 216;
//     }
//     vdp_cmd_await();
//     vmem_write(SPRITES + sizeof(sprites[0]) * beg,
//                (void*)&(sprites[beg]),
//                sizeof(sprites[0]) * (endIdx - beg));
//     anime_update();
//     sm2_clear_sprites();
//   }
// }

void sm2_apply_sprites(void) {
  if (sm2_is_dirty()) {
    vdp_cmd_await();
    // vmem_write(SPRITES, (void*)&sprites, sizeof(sprites));
    vmem_write(SPRITES + sizeof(sprites[0]) * 2, (void*)&(sprites[2]), sizeof(sprites[0]) * 30);
    anime_update();
    sm2_clear_sprites();
  }
}

bool sm2_is_dirty(void) {
  return (beg < end);
}
