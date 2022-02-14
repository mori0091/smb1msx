// -*- coding: utf-8-unix -*-

#include "smb1.h"
#include "sm2.h"

#define ENTITY_MAX  (8)

struct {
  uint8_t length;
  entity_t * list[ENTITY_MAX];
} entities;

entity_t player_entity;
entity_t * const player = &player_entity;

#define BUFFER_COLS   (STAGEMAP_BUFFER_PAGES * STAGEMAP_PAGE_COLS)
#define BUFFER_WIDTH  (BUFFER_COLS * TILE_WIDTH)

void entity_init(void) {
  entities.length = 1;
  entities.list[0] = player;
}

void entity_add(entity_t * const e) {
  if (ENTITY_MAX <= entities.length || !e) return;
  entities.list[entities.length++] = e;
}

void entity_remove(entity_t * const e) {
  if (!entities.length || !e) return;
  if (e->metasprite) {
    sm2_hide_sprites(e->plane, e->metasprite);
  }
  entity_t ** p = &(entities.list[entities.length - 1]);
  entity_t * const last = *p;
  uint8_t n = entities.length;
  while (n--) {
    if (*p == e) {
      entities.length--;
      *p = last;
      return;
    }
    p--;
  }
}

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

void entity_update_collision(entity_t * e) {
  uint8_t collision = 0;
  uint8_t c1 = 0;
  uint8_t c2 = 0;

  const int16_t x = e->pos.x.i;
  const int16_t y = e->pos.y.i;
  const int8_t diff_y = y - e->prev_pos.y.i;
  const uint8_t off_x = x & 15;
  const uint8_t off_y = y & 15;

  // Tiles with overlapping entity `e`.
  uint8_t o00 = mapld_get_object_at(x   , y);
  uint8_t o01 = mapld_get_object_at(x+16, y);
  uint8_t o10 = mapld_get_object_at(x   , y+16);
  uint8_t o11 = mapld_get_object_at(x+16, y+16);

  // check ceil
  if (diff_y < 0) {
    c1 = mapld_get_object_at(x+CEIL_MARGIN_L, y);
    c2 = mapld_get_object_at(x+15-CEIL_MARGIN_R, y);
    if ((c1 | c2) & 0x80) {
      collision |= COLLISION_CEIL;
      o00 = o10;                // for left / right collision check
      o01 = o11;
    }
  }
  // check floor
  else {
    c1 = mapld_get_object_at(x+FLOOR_MARGIN_L, y+16);
    c2 = mapld_get_object_at(x+15-FLOOR_MARGIN_R, y+16);
    if ((0x7f < c1 && c1 < 0xff) || (0x7f < c2 && c2 < 0xff)) {
      collision |= COLLISION_FLOOR;
      o10 = o00;                // for left / right collision check
      o11 = o01;
    }
  }

  {
    uint8_t o1 = 0;
    uint8_t o2 = 0;

    // check left-side wall
    if (off_x < SIDE_L) {
      {
        o1 = o00;
      }
      if (off_y) {
        o2 = o10;
      }
      if ((0x7f < o1 && o1 < 0xff) || (0x7f < o2 && o2 < 0xff)) {
        if (c2 & 0x80) {
          collision |= COLLISION_LEFT;
        }
        else {
          collision = COLLISION_LEFT;
        }
        e->pos.x.i = (x & ~15) + SIDE_L;
        e->pos.x.d = 0;
        e->vel.x = 0;
      }
    }

    // check right-side wall
    if (off_x > SIDE_R) {
      {
        o1 = o01;
      }
      if (off_y) {
        o2 = o11;
      }
      if ((0x7f < o1 && o1 < 0xff) || (0x7f < o2 && o2 < 0xff)) {
        if (c1 & 0x80) {
          collision |= COLLISION_RIGHT;
        }
        else {
          collision = COLLISION_RIGHT;
        }
        e->pos.x.i = (x & ~15) + SIDE_R;
        e->pos.x.d = 0;
        e->vel.x = 0;
      }
    }
  }

  // correct Y-coordinates
  if (collision & COLLISION_CEIL) {
    e->pos.y.i = (y & 240) + 16;
    e->pos.y.d = 0;
    e->vel.y = abs(e->vel.y);
  } else if (collision & COLLISION_FLOOR) {
    e->pos.y.i = (y & 240);
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

inline void player_correct_position(void) {
  // Limits the state of the dynamics to between the upper and lower limits.
  uint16_t x0 = camera_get_x() + 8;
  if (player->pos.x.i < x0) {
    player->pos.x.i = x0;
    player->pos.x.d = 0;
    player->vel.x = 0;
  }
}

static void foreach_active_entity(void (* f)(entity_t *)) {
  entity_t ** es = entities.list;
  uint8_t n = entities.length;
  while (n--) {
    f(*es++);
  }
}

void entity_update(void) {
  // Action planning task
  foreach_active_entity(entity_update_input);
  // Dynamics state update task (position)
  foreach_active_entity(entity_update_dynamics);
  player_correct_position();
  // Collision test task
  foreach_active_entity(entity_update_collision);
  // Dynamics state update task (velocity / acceleration)
  foreach_active_entity(entity_update_speed);
  // update camera position and speed
  camera_update();
  // Pose update & Sound / Visual effects, etc.
  foreach_active_entity(entity_run_post_step);
}

static void entity_set_sprites(entity_t * e) {
  if (!e->metasprite) return;
  const int x = e->pos.x.i - camera_get_x();
  const int y = e->pos.y.i - 1;
  sm2_show_sprites(e->plane, e->metasprite, x, y);
}

void entity_update_sprites(void) {
  sm2_clear_sprites();
  foreach_active_entity(entity_set_sprites);
}

void entity_apply_sprites(void) {
  sm2_apply_sprites();
}

// void entity_update_sprites(void) {
//   entity_t ** es = entities.list;
//   uint8_t n = entities.length;
//   uint8_t plane = 0;
//   while (n--) {
//     entity_t * e = *es++;
//     if (!e->metasprite) continue;
//     assert(plane + e->metasprite->n <= 32);
//     if (e->plane != plane) {
//       e->plane = plane;
//       assets_set_sprite_palette(SPRITES_0, plane, e->sp_palette);
//     }
//     plane += e->metasprite->n;
//     entity_show_sprite(e);
//   }
// }
