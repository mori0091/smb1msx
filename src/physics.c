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
      c1 = mapld_get_object_at(box.left+CEIL_MARGIN_L, box.top);
      c2 = mapld_get_object_at(box.right-CEIL_MARGIN_R, box.top);
      if ((c1 | c2) & 0x80) {
        collision |= COLLISION_CEIL;
      }
    }
    // check floor
    else {
      c1 = mapld_get_object_at(box.left+FLOOR_MARGIN_L, box.bottom+1);
      c2 = mapld_get_object_at(box.right-FLOOR_MARGIN_R, box.bottom+1);
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
      if (is_obstacle(mapld_get_object_at(xl, yt)) ||
          is_obstacle(mapld_get_object_at(xl, yc)) ||
          is_obstacle(mapld_get_object_at(xl, yb))) {
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
      if (is_obstacle(mapld_get_object_at(xr, yt)) ||
          is_obstacle(mapld_get_object_at(xr, yc)) ||
          is_obstacle(mapld_get_object_at(xr, yb))) {
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
