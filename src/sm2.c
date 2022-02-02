// -*- coding: utf-8-unix -*-

#include "smb1.h"

static void sprites_from_metasprite(struct sprite * sp,
                                    const metasprite_t * ms
                                    , int x, int y) {
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

struct sprite sprites[32];

// dirty range [beg, end)
static uint8_t beg;
static uint8_t end;
// dirty flag
static bool dirty;

static void on_modified(uint8_t plane, uint8_t n) {
  uint8_t last = plane + n;
  if (end < last) {
    end = last;
  }
  if (plane < beg) {
    beg = plane;
  }
  dirty = true;
}

void sm2_init(void) {
  beg = 255;
  end = 0;
  dirty = false;
}

void sm2_add_sprites(uint8_t plane, const metasprite_t * ms, int x, int y) {
  // assert(ms && plane + ms->n <= 32);
  sprites_from_metasprite(sprites + plane, ms, x, y);
  on_modified(plane, ms->n);
}

void sm2_apply_sprites(void) {
  if (dirty) {
    // \note Don't change [beg, end) range.
    uint8_t endIdx = end;
    if (endIdx < sizeof(sprites)/sizeof(sprites[0])) {
      // Hides all the sprites whose plane number is greater than or equal to
      // endIdx.
      sprites[endIdx++].y = 216;
    }
    vdp_cmd_await();
    vmem_write(SPRITES_0 + sizeof(sprites[0]) * beg,
               (void*)&(sprites[beg]),
               sizeof(sprites[0]) * (endIdx - beg));
    anime_update();
    dirty = false;
  }
  else {
    if (mario_is_weakened()) {
      vdp_cmd_await();
      entity_hide_sprite(player);
    }
  }
}
