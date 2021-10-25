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

// static const tagged_color_t mario_colors[] = {
//   1,  2  | SPRITE_TAG_CC,       // Mario colors
//   14, 13 | SPRITE_TAG_CC,       // Fire colors
//   14, 5  | SPRITE_TAG_CC,       // Luigi colors
//   10, 9  | SPRITE_TAG_CC,       // Goomba colors
// };

static const tagged_color_t mario_colors[] = {
  1, 2 | SPRITE_TAG_CC,         // Mario colors
};

void mario_init(void) {
  mario_state.input = 0;
  mario_state.speed = 0;
  mario_state.facing = FACING_RIGHT;

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
  vmem_set_sprite_color_m(SPRITES, 0, mario_metasprite.n, mario_colors);
}

void mario_animate(void) {
  if (mario_state.speed == 0) {
    vmem_write(SPRITE_PATTERNS+0x000,
               smb1spt + 64 * (STANDING + mario_state.facing),
               64);
  } else {
    const uint8_t b = (mario_state.input & VK_FIRE_1) ? 1 : 2;
    if (!(tick & ((1 << b) - 1))) {
      const uint8_t j = (tick >> b) % 3;
      vmem_write(SPRITE_PATTERNS+0x000,
                 smb1spt + 64 * (2*j + (WALKING_1 + mario_state.facing)),
                 64);
    }
  }

  if ((tick & 1)) return;
  /* move sprite */
  const int16_t x = mario_state.dynamics_state.pos.x.i - camera_get_x();
  const int16_t y = mario_state.dynamics_state.pos.y.i;
  vmem_set_metasprite_a(SPRITES, 0, x, y, &mario_metasprite);
}

static void mario_update_input_state(void) {
  const uint8_t joy = joypad_get_state(1);
  mario_state.input &= 0x30;
  mario_state.input <<= 2;
  mario_state.input |= joy & 0x3f;
}

static void mario_update_speed_on_floor(void) {
  const f10q6_t speed_hi = f10q6i(10);
  const f10q6_t speed_lo = f10q6i(6);
  const uint8_t accel = 16;
  const uint8_t brake = 24;

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
    if (mario_state.input & VK_FIRE_1) {
      mario_state.speed += 2 * accel;
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
  // TODO
}

static void mario_update_speed(void) {
  if (mario_state.collision_state & COLLISION_FLOOR) {
    mario_update_speed_on_floor();
  } else {
    mario_update_speed_flight();
  }
}

void mario_move(void) {
  mario_update_input_state();

  dynamics_state_update(&mario_state.dynamics_state);
  if (mario_state.dynamics_state.pos.x.i < camera_get_x() + 8) {
    mario_state.dynamics_state.pos.x.i = camera_get_x() + 8;
    mario_state.dynamics_state.pos.x.d = 0;
  }

  mario_update_speed();
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
