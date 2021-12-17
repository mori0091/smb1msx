// -*- coding: utf-8-unix -*-

#include "smb1.h"

#define A_BUTTON       VK_FIRE_0
#define B_BUTTON       VK_FIRE_1
#define PREV_A_BUTTON  (VK_FIRE_0 << 2)
#define PREV_B_BUTTON  (VK_FIRE_1 << 2)

// #define speed_hi       f10q6i(8)
// #define speed_lo       f10q6(4.8)
#define speed_hi       f10q6i(6)
#define speed_lo       f10q6(3.6)
#define accel_hi       ((uint8_t)20)
#define accel          ((uint8_t)10)
#define brake          ((uint8_t)24)
#define initial_vy_hi  f10q6(-8.25)
#define initial_vy_lo  f10q6(-8.0)
#define gravity_hi     f10q6(2.0)
#define gravity_lo     f10q6(0.5)

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

struct mario_state mario_state;

static const vec2i_t W16H16D2[] = {
  /* layer #1 */
  {0,0},
  /* layer #2 */
  {0,0},
};

static const uint8_t mario_pats[] = {
  /* layer #1 */
  0,
  /* layer #2 */
  4,
};

static const metasprite_t mario_metasprite = {
  .n = 2,
  .anchor = {0,0},
  .layouts = W16H16D2,
  .pats = mario_pats,
};

void mario_init(void) {
  // mario_state.life = 3;
  mario_state.input = 0;
  mario_state.speed = 0;
  mario_state.facing = FACING_RIGHT;
  mario_state.pose = STANDING;

  /* SDCC does not support ISO C99 compound literal */
  mario_state.dynamics_state.pos.x.i = 40;
  mario_state.dynamics_state.pos.x.d = 0;
  mario_state.dynamics_state.pos.y.i = 176;
  mario_state.dynamics_state.pos.y.d = 0;
  mario_state.dynamics_state.vel.x = 0;
  mario_state.dynamics_state.vel.y = 0;
  mario_state.dynamics_state.acc.x = 0;
  mario_state.dynamics_state.acc.y = 0;

  mario_state.collision_state = COLLISION_FLOOR;
  /* assets_set_sprite_palette(SPRITES, 0, MARIO_PALETTE); */
}

#define SPT(pat)    (smb1spt + 64 * (pat))
inline void mario_set_sprite_pat(uint8_t pat) {
  graphics_set_sprite_pat(0, SPT(pat), 64);
}

inline void mario_move_sprite(void) {
  const int16_t x = mario_state.dynamics_state.pos.x.i - camera_get_x();
  const int16_t y = mario_state.dynamics_state.pos.y.i;
  vmem_set_metasprite_a(SPRITES, 0, x, y, &mario_metasprite);
}

static uint8_t anim_tick;

void mario_animate(void) {
  if (mario_state.pose == WALKING) {
    if (speed_lo < mario_state.speed) {
      anim_tick += 2;
    }
    else {
      anim_tick++;
    }
    if (6 <= anim_tick) {
      anim_tick = 0;
    }
    const uint8_t t = anim_tick & ~1;
    mario_set_sprite_pat(t + WALKING + mario_state.facing);
  }
  else {
    mario_set_sprite_pat(mario_state.pose + mario_state.facing);
  }

  mario_move_sprite();
}

void mario_animate_die(void) {
  mario_state.pose = DEAD;
  mario_state.facing = 0;
  mario_state.input = A_BUTTON;
  mario_state.speed = 0;
  if (212 - 16 < mario_state.dynamics_state.pos.y.i) {
    // falling down
    mario_state.dynamics_state.vel.y = f10q6(0.0);
    mario_state.dynamics_state.acc.y = gravity_lo;
  }
  else {
    // ouch!
    mario_state.dynamics_state.vel.y = f10q6(-20.0);
    mario_state.dynamics_state.acc.y = gravity_hi;
  }
  mario_state.dynamics_state.vel.x = 0;
  mario_state.dynamics_state.acc.x = 0;

  await_hsync();
  mario_animate();
  sleep_millis(500);
  do {
    tick++;
    await_hsync();
    mario_animate();
    if (tick & 1) continue;
    dynamics_state_update(&mario_state.dynamics_state);
  } while (mario_state.dynamics_state.pos.y.i <= 240);
}

void mario_backup_input_state(void) {
  mario_state.input &= (VK_FIRE_0 | VK_FIRE_1);
  mario_state.input <<= 2;
}

void mario_update_input_state(void) {
  mario_backup_input_state();
  mario_state.input |= joypad_get_state(1);
}

static void mario_update_speed_on_floor(void) {
  {
    if (mario_state.dynamics_state.vel.x < 0) {
      mario_state.facing = FACING_LEFT;
    }
    if (0 < mario_state.dynamics_state.vel.x) {
      mario_state.facing = FACING_RIGHT;
    }
    mario_state.speed = abs(mario_state.dynamics_state.vel.x);
  }

  uint8_t FORWARD_KEY;
  uint8_t BACKWARD_KEY;
  if (mario_state.facing == FACING_LEFT) {
    FORWARD_KEY = VK_LEFT;
    BACKWARD_KEY = VK_RIGHT;
  } else {
    FORWARD_KEY = VK_RIGHT;
    BACKWARD_KEY = VK_LEFT;
  }

  const uint8_t LR_KEY = mario_state.input & (VK_LEFT | VK_RIGHT);

  if (LR_KEY == FORWARD_KEY) {
    if (mario_state.input & B_BUTTON) {
      mario_state.speed += accel_hi;
      if (speed_hi <= mario_state.speed) {
        mario_state.speed = speed_hi;
      }
    } else if (mario_state.speed < speed_lo) {
      mario_state.speed += accel;
    } else {
      mario_state.speed -= accel;
    }
  }
  else if (LR_KEY == BACKWARD_KEY) {
    if (brake < mario_state.speed) {
      mario_state.speed -= brake;
    } else {
      mario_state.speed = 0;
    }
  }
  else {
    if (accel < mario_state.speed) {
      mario_state.speed -= accel;
    } else {
      mario_state.speed = 0;
    }
  }

  if (!mario_state.speed) {
    if (LR_KEY == VK_LEFT) {
      mario_state.facing = FACING_LEFT;
    }
    if (LR_KEY == VK_RIGHT) {
      mario_state.facing = FACING_RIGHT;
    }
  }

  if (mario_state.facing == FACING_LEFT) {
    mario_state.dynamics_state.vel.x = -mario_state.speed;
  } else {
    mario_state.dynamics_state.vel.x = mario_state.speed;
  }
}

static void mario_update_speed_flight(void) {
  const uint8_t LR_KEY = mario_state.input & (VK_LEFT | VK_RIGHT);

  if (LR_KEY == VK_RIGHT) {
    if (mario_state.input & B_BUTTON) {
      // mario_state.dynamics_state.vel.x += accel_hi;
      mario_state.dynamics_state.vel.x += accel;
      if (speed_hi <= mario_state.dynamics_state.vel.x) {
        mario_state.dynamics_state.vel.x = speed_hi;
      }
    } else if (mario_state.dynamics_state.vel.x < speed_lo) {
      mario_state.dynamics_state.vel.x += accel;
    } else {
      // mario_state.dynamics_state.vel.x -= accel;
    }
  }
  else if (LR_KEY == VK_LEFT) {
    if (mario_state.input & B_BUTTON) {
      // mario_state.dynamics_state.vel.x -= accel_hi;
      mario_state.dynamics_state.vel.x -= accel;
      if (mario_state.dynamics_state.vel.x <= -speed_hi) {
        mario_state.dynamics_state.vel.x = -speed_hi;
      }
    } else if (-speed_lo < mario_state.dynamics_state.vel.x) {
      mario_state.dynamics_state.vel.x -= accel;
    } else {
      // mario_state.dynamics_state.vel.x += accel;
    }
  }
  mario_state.speed = abs(mario_state.dynamics_state.vel.x);
}

static void mario_update_speed(void) {
  if (mario_state.collision_state & COLLISION_FLOOR) {
    // update horizontal speed
    mario_update_speed_on_floor();
    // jump (set initial vertical velocity and gravity)
    if ((mario_state.input & (A_BUTTON | PREV_A_BUTTON)) == A_BUTTON) {
      if (mario_state.input & B_BUTTON) {
        mario_state.dynamics_state.vel.y = initial_vy_hi;
      } else {
        mario_state.dynamics_state.vel.y = initial_vy_lo;
      }
      mario_state.dynamics_state.acc.y = gravity_lo;
      // estimate mario's pose
      mario_state.pose = JUMPING;
      // ---- sound effect ----
      sound_effect(&se_jump);
    }
    // estimate mario's pose
    else if (mario_state.speed == 0) {
      mario_state.pose = STANDING;
    }
    else {
      mario_state.pose = WALKING;
    }
  } else {
    // update horizontal speed
    mario_update_speed_flight();
    // hit the ceil?
    if (mario_state.collision_state & COLLISION_CEIL) {
      // ---- sound effect ----
      sound_effect(&se_block);
    }
    // jump (control gravity)
    if (!(mario_state.input & A_BUTTON) ||
        0 <= mario_state.dynamics_state.vel.y) {
      mario_state.dynamics_state.acc.y = gravity_hi;
    }
  }
}

void mario_move(void) {
  dynamics_state_update(&mario_state.dynamics_state);

  {
    // Limits the state of the dynamics to between the upper and lower limits.
    uint16_t x0 = camera_get_x() + 8;
    if (mario_state.dynamics_state.pos.x.i < x0) {
      mario_state.dynamics_state.pos.x.i = x0;
      mario_state.dynamics_state.pos.x.d = 0;
    }
    if (mario_state.dynamics_state.vel.y < f10q6i(-16)) {
      mario_state.dynamics_state.vel.y = f10q6i(-16);
    }
    if (f10q6i(16) < mario_state.dynamics_state.vel.y) {
      mario_state.dynamics_state.vel.y = f10q6i(16);
    }
    if (mario_state.dynamics_state.pos.y.i < -32) {
      mario_state.dynamics_state.pos.y.i = -32;
      mario_state.dynamics_state.pos.y.d = 0;
    }
    if (211 < mario_state.dynamics_state.pos.y.i) {
      mario_state.dynamics_state.pos.y.i = 212;
      mario_state.dynamics_state.pos.y.d = 0;
      event_set(EV_PLAYER_DIES);
    }
  }

  collision_state_update(&mario_state.collision_state,
                         &mario_state.dynamics_state);

  mario_update_speed();

  // ---- !? Mario can go back to the left of starting page...??
  if (mario_state.dynamics_state.pos.x.i <= 8) {
    mario_state.dynamics_state.pos.x.i = 8;
    mario_state.dynamics_state.pos.x.d = 0;
  }
}

int16_t mario_get_prev_x(void) {
  return mario_state.dynamics_state.prev_pos.x.i;
}

int16_t mario_get_x(void) {
  return mario_state.dynamics_state.pos.x.i;
}

void mario_set_x(int16_t x) {
  mario_state.dynamics_state.pos.x.i = x;
}
