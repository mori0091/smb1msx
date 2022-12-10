// -*- coding: utf-8-unix -*-

#pragma codeseg BANK1

#include "smb1.h"
#include <stdint.h>

const vec2i_t W16H16D2[] = { {0,0}, {0,0}, };

uint8_t no_controller(void) {
  return 0;
}

uint8_t joystick1(void) {
  return joypad_get_state(1);
}

void entity_set_controller(entity_t * e, controller_t c) {
  if (!e) return;
  if (c) {
    e->controller = c;
  }
  else {
    e->controller = no_controller;
  }
}

void entity_get_bounds(const entity_t * e, rect_t * rect) {
  if (e == player && mario_has_super_ability() && mario_state.pose != CROUCHING) {
    rect->left   = player->pos.x.i;
    rect->top    = player->pos.y.i - 16;
    rect->right  = rect->left + 15;
    rect->bottom = rect->top  + 31;
  }
  else {
    rect->left   = e->pos.x.i;
    rect->top    = e->pos.y.i;
    rect->right  = rect->left + 15;
    rect->bottom = rect->top  + 15;
  }
}

void entity_update_input(entity_t * e) {
  e->input &= (VK_FIRE_0 | VK_FIRE_1);
  e->input <<= 2;
  e->input |= e->controller();
}

void entity_update_dynamics(entity_t * e) {
  /* SDCC does not support assignment to variable of struct / union. */
  e->prev_pos.x.i = e->pos.x.i;
  e->prev_pos.x.d = e->pos.x.d;
  e->prev_pos.y.i = e->pos.y.i;
  e->prev_pos.y.d = e->pos.y.d;

  f8q8_t f;

  f = e->pos.x.d + e->vel.x;
  e->pos.x.i += f >> 8;
  e->pos.x.d = f & 255;

  f = e->pos.y.d + e->vel.y;
  e->pos.y.i += f >> 8;
  e->pos.y.d = f & 255;

  e->vel.x += e->acc.x;
  e->vel.y += e->acc.y;

  /* correct y-axis position and velocity */
  if (e->vel.y < f8q8i(-16)) {
    e->vel.y = f8q8i(-16);
  }
  if (f8q8i(16) < e->vel.y) {
    e->vel.y = f8q8i(16);
  }
  if (e->pos.y.i < -32) {
    e->pos.y.i = -32;
    e->pos.y.d = 0;
  }
  if (240 <= e->pos.y.i) {
    e->pos.y.i = 240;
    e->pos.y.d = 0;
  }
}

// for ceil collision test
#define CEIL_MARGIN_L   (6)
#define CEIL_MARGIN_R   (6)
#define CEIL_L          (TILE_WIDTH - CEIL_MARGIN_L)
#define CEIL_R          (CEIL_MARGIN_R)
// for floor collision test
#define FLOOR_MARGIN_L  (3)
#define FLOOR_MARGIN_R  (3)
#define FLOOR_L         (TILE_WIDTH - FLOOR_MARGIN_L)
#define FLOOR_R         (FLOOR_MARGIN_R)
// for side-wall collision test
#define SIDE_MARGIN_L   (2)
#define SIDE_MARGIN_R   (2)
#define SIDE_L          (TILE_WIDTH - SIDE_MARGIN_L)
#define SIDE_R          (SIDE_MARGIN_R)

inline bool is_obstacle(uint8_t obj) {
  return (0x7f < obj && obj < 0xff);
}

void entity_update_collision(entity_t * e) {
  static rect_t box;
  entity_get_bounds(e, &box);

  static uint8_t collision, c1, c2;
  collision = 0;
  c1 = 0;
  c2 = 0;

  {
    int8_t diff_y = e->pos.y.i - e->prev_pos.y.i;
    // check ceil
    if (diff_y < 0) {
      c1 = map_get_object_at(box.left+CEIL_MARGIN_L, box.top);
      c2 = map_get_object_at(box.right-CEIL_MARGIN_R, box.top);
      if ((c1 | c2) & 0x80) {
        collision |= COLLISION_CEIL;
      }
    }
    // check floor
    else {
      c1 = map_get_object_at(box.left+FLOOR_MARGIN_L, box.bottom+1);
      c2 = map_get_object_at(box.right-FLOOR_MARGIN_R, box.bottom+1);
      if (is_obstacle(c1) || is_obstacle(c2)) {
        collision |= COLLISION_FLOOR;
      }
    }
  }

  {
    int8_t dy = 0;
    if (collision & COLLISION_CEIL) {
      dy = TILE_HEIGHT;
    }
    if (collision & COLLISION_FLOOR) {
      dy = 1 - TILE_HEIGHT;
    }

    uint16_t yt = box.top + dy;
    uint16_t yb = box.bottom + dy;
    uint16_t yc = (yt+yb)/2;

    uint8_t off_x = box.left & 15;
    // check left-side wall
    if (off_x < SIDE_L) {
      uint16_t xl = box.left + SIDE_MARGIN_L;
      if (is_obstacle(map_get_object_at(xl, yt)) ||
          is_obstacle(map_get_object_at(xl, yc)) ||
          is_obstacle(map_get_object_at(xl, yb))) {
        if (c2 & 0x80) {
          collision |= COLLISION_LEFT;
        }
        else {
          collision = COLLISION_LEFT;
        }
      }
    }
    // check right-side wall
    if (off_x > SIDE_R) {
      uint16_t xr = box.right - SIDE_MARGIN_R;
      if (is_obstacle(map_get_object_at(xr, yt)) ||
          is_obstacle(map_get_object_at(xr, yc)) ||
          is_obstacle(map_get_object_at(xr, yb))) {
        if (c1 & 0x80) {
          collision |= COLLISION_RIGHT;
        }
        else {
          collision = COLLISION_RIGHT;
        }
      }
    }
  }

  // correct X-coordinates
  if (collision & (COLLISION_LEFT | COLLISION_RIGHT)) {
    e->pos.x.i &= ~15;
    e->pos.x.d = 0;
    e->vel.x = 0;
    if (collision & COLLISION_LEFT) {
      e->pos.x.i += SIDE_L;
    }
    else {
      e->pos.x.i += SIDE_R;
    }
  }
  // correct Y-coordinates
  if (collision & COLLISION_CEIL) {
    e->pos.y.i = (e->pos.y.i & 240) + 16;
    e->pos.y.d = 0;
    e->vel.y = abs(e->vel.y);
  }
  if (collision & COLLISION_FLOOR) {
    e->pos.y.i = (e->pos.y.i & 240);
    e->pos.y.d = 0;
    e->vel.y = 0;
  }

  e->collision = collision;
  e->c1 = c1;
  e->c2 = c2;
}

static void entity_update_speed_on_floor(entity_t * e) {
  {
    if (e->vel.x < 0) {
      entity_set_facing(e, FACING_LEFT);
    }
    if (0 < e->vel.x) {
      entity_set_facing(e, FACING_RIGHT);
    }
  }
  f8q8_t speed = abs(e->vel.x);

  uint8_t FORWARD_KEY;
  uint8_t BACKWARD_KEY;
  if (entity_get_facing(e) == FACING_LEFT) {
    FORWARD_KEY = VK_LEFT;
    BACKWARD_KEY = VK_RIGHT;
  } else {
    FORWARD_KEY = VK_RIGHT;
    BACKWARD_KEY = VK_LEFT;
  }

  const uint8_t LR_KEY = e->input & (VK_LEFT | VK_RIGHT);

  e->braking = false;
  if (LR_KEY == FORWARD_KEY) {
    if (e->input & B_BUTTON) {
      speed += accel_hi;
      if (speed_hi <= speed) {
        speed = speed_hi;
      }
    } else if (speed < speed_lo) {
      speed += accel;
    } else {
      speed -= accel;
    }
  }
  else if (LR_KEY == BACKWARD_KEY) {
    if (brake < speed) {
      speed -= brake;
      e->braking = true;
    } else {
      speed = 0;
    }
  }
  else {
    if (accel < speed) {
      speed -= accel;
    } else {
      speed = 0;
    }
  }

  if (!speed) {
    if (LR_KEY == VK_LEFT) {
      entity_set_facing(e, FACING_LEFT);
    }
    if (LR_KEY == VK_RIGHT) {
      entity_set_facing(e, FACING_RIGHT);
    }
  }

  if (entity_get_facing(e) == FACING_LEFT) {
    e->vel.x = -speed;
  } else {
    e->vel.x = speed;
  }
}

static void entity_update_speed_flight(entity_t * e) {
  const uint8_t LR_KEY = e->input & (VK_LEFT | VK_RIGHT);

  if (LR_KEY == VK_RIGHT) {
    if (e->input & B_BUTTON) {
      // e->vel.x += accel_hi;
      e->vel.x += accel;
      if (speed_hi <= e->vel.x) {
        e->vel.x = speed_hi;
      }
    } else if (e->vel.x < speed_lo) {
      e->vel.x += accel;
    } else {
      // e->vel.x -= accel;
    }
  }
  else if (LR_KEY == VK_LEFT) {
    if (e->input & B_BUTTON) {
      // e->vel.x -= accel_hi;
      e->vel.x -= accel;
      if (e->vel.x <= -speed_hi) {
        e->vel.x = -speed_hi;
      }
    } else if (-speed_lo < e->vel.x) {
      e->vel.x -= accel;
    } else {
      // e->vel.x += accel;
    }
  }
}

void entity_update_speed(entity_t * e) {
  if (e->collision & COLLISION_FLOOR) {
    // update horizontal speed
    entity_update_speed_on_floor(e);
    // jump (set initial vertical velocity and gravity)
    if ((e->input & (A_BUTTON | PREV_A_BUTTON)) == A_BUTTON) {
      if (e->input & B_BUTTON) {
        e->vel.y = initial_vy_hi;
      } else {
        e->vel.y = initial_vy_lo;
      }
      e->acc.y = gravity_lo;
    }
  } else {
    // update horizontal speed
    entity_update_speed_flight(e);
    // jump (control gravity)
    if (!(e->input & A_BUTTON) || 0 <= e->vel.y) {
      e->acc.y = gravity_hi;
    }
  }
}

void entity_run_post_step(entity_t * e) {
  e->post_step(e);
}
