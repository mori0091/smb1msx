// -*- coding: utf-8-unix -*-

#include "smb1.h"

void dynamics_state_update(dynamics_state_t* ds) {
  /* SDCC does not support assignment to variable of struct / union. */
  ds->prev_pos.x.i = ds->pos.x.i;
  ds->prev_pos.x.d = ds->pos.x.d;
  ds->prev_pos.y.i = ds->pos.y.i;
  ds->prev_pos.y.d = ds->pos.y.d;

  ds->pos.x.d += ds->vel.x;
  ds->pos.x.i += ds->pos.x.d >> 6;
  ds->pos.x.d &= ((1 << 6) - 1);

  ds->pos.y.d += ds->vel.y;
  ds->pos.y.i += ds->pos.y.d >> 6;
  ds->pos.y.d &= ((1 << 6) - 1);

  ds->vel.x += ds->acc.x;
  ds->vel.y += ds->acc.y;
}

void collision_state_update(collision_state_t* cs, dynamics_state_t* ds) {
  static uint8_t c1;            // ceil / floor object #1
  static uint8_t c2;            // ceil / floor object #2
  static f16q6_t yy;            // Candidate for corrected Y-coordinate position.
  static f10q6_t vy;            // Candidate for corrected Y-coordinate velocity.

  c1 = 0;
  c2 = 0;
  *cs = 0;
  if (ds->pos.y.i <= -16) {
    return;
  }

  const uint16_t x0 = ds->pos.x.i % stage_get_width();
  yy.i = ds->pos.y.i;
  yy.d = ds->pos.y.d;
  vy = ds->vel.y;

  {
    if (ds->pos.y.i < ds->prev_pos.y.i) {
      // - collision check (ceil)
      static const rect_t box = {
        .pos  = { 6, 0 },
        .size = { 4,16 },
      };
      const int lx = x0 + box.pos.x;
      const int rx = lx + box.size.x - 1;
      const int ty = ds->pos.y.i + box.pos.y - 1;
      c1 = stage_get_object_at(lx, ty);
      c2 = stage_get_object_at(rx, ty);
      if ((c1 | c2) & 0x80) {
        *cs |= COLLISION_CEIL;
        yy.i = (ds->pos.y.i + 15) & 240;
        yy.d = 0;
        vy = abs(ds->vel.y);
      }
    }
    else {
      // - collision check (floor)
      static const rect_t box = {
        .pos  = { 3, 0 },
        .size = { 10,16 },
      };
      const int lx = x0 + box.pos.x;
      const int rx = lx + box.size.x - 1;
      const int by = ds->pos.y.i + box.pos.y + box.size.y;
      c1 = stage_get_object_at(lx, by);
      c2 = stage_get_object_at(rx, by);
      if ((c1 | c2) & 0x80) {
        *cs |= COLLISION_FLOOR;
        yy.i = ds->pos.y.i & 240;
        yy.d = 0;
        vy = 0;
      }
    }
  }
  {
    // - collision check (left / right)
    static const rect_t box = {
      .pos  = { 2, 0 },
      .size = { 12,16 },
    };

    bool check_right;
    if (ds->prev_pos.x.i == ds->pos.x.i) {
      check_right = (ds->pos.x.i & 15) < 8;
    } else {
      check_right = ds->prev_pos.x.i < ds->pos.x.i;
    }

    uint16_t xa;
    uint16_t xb;
    uint8_t a;
    uint8_t b;
    if (check_right) {
      xa = x0 + box.pos.x + box.size.x - 1;
      xb = ((ds->pos.x.i +  0) & 0x0fff0) - box.pos.x - box.size.x + 16;
      a = c1;
      b = COLLISION_RIGHT;
    } else {
      xa = x0 + box.pos.x;
      xb = ((ds->pos.x.i + 15) & 0x0fff0) - box.pos.x;
      a = c2;
      b = COLLISION_LEFT;
    }
    const uint8_t c = (stage_get_object_at(xa, yy.i + 0 ) |
                       stage_get_object_at(xa, yy.i + 15));
    if (c & 0x080) {
      ds->pos.x.i = xb;
      ds->pos.x.d = 0;
      ds->vel.x = 0;
      *cs |= b;
      // recheck ceil/floor, then clear flag or apply y/vy
      if (!(a & 0x080)) {
        *cs &= ~(COLLISION_FLOOR | COLLISION_CEIL);
        return;
      }
    }
  }
  ds->pos.y.i = yy.i;
  ds->pos.y.d = yy.d;
  ds->vel.y = vy;
}
