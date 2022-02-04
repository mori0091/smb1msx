// -*- coding: utf-8-unix -*-

#include "smb1.h"
#include "sm2.h"

#define ENTITY_MAX  (8)

struct {
  uint8_t length;
  entity_t * list[ENTITY_MAX];
} entities;

static bool updated;

entity_t player_entity;
entity_t * const player = &player_entity;

const struct sprite hidden_sprite = {.y = 240, };

#define BUFFER_COLS   (STAGEMAP_BUFFER_PAGES * STAGEMAP_PAGE_COLS)
#define BUFFER_WIDTH  (BUFFER_COLS * TILE_WIDTH)

void entity_show_sprite(const entity_t * e) {
  const int16_t x = e->pos.x.i - camera_get_x();
  const int16_t y = e->pos.y.i - 1;
  vmem_set_metasprite_a(SPRITES_0, e->plane, x, y, e->metasprite);
}

void entity_hide_sprite(const entity_t * e) {
  uint8_t plane = e->plane;
  uint8_t n = e->metasprite->n;
  while (n--) {
    vmem_set_sprite(SPRITES_0, plane++, &hidden_sprite);
  }
}

void entity_init(void) {
  entities.length = 1;
  entities.list[0] = player;
}

void entity_add(entity_t * const e) {
  if (ENTITY_MAX <= entities.length || !e) return;
  entities.list[entities.length++] = e;
  // entity_show_sprite(e);
}

void entity_remove(entity_t * const e) {
  if (!entities.length || !e) return;
  // entity_hide_sprite(e);
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
  // e->pos.x.i += f >> 6;
  // e->pos.x.d = f & ((1 << 6) - 1);
  e->pos.x.i += f >> 8;
  e->pos.x.d = f & 255;

  f = e->pos.y.d + e->vel.y;
  // e->pos.y.i += f >> 6;
  // e->pos.y.d = f & ((1 << 6) - 1);
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

void entity_update_collision(entity_t * e) {
  static uint8_t c1;           // ceil / floor object #1
  static uint8_t c2;           // ceil / floor object #2
  static f16q8_t yy;           // Candidate for corrected Y-coordinate position.
  static f8q8_t vy;            // Candidate for corrected Y-coordinate velocity.

  c1 = 0;
  c2 = 0;
  e->collision = 0;
  if (e->pos.y.i <= -16) {
    return;
  }

  const uint16_t x = e->pos.x.i;
  const uint16_t prev_x = e->prev_pos.x.i;
  const uint16_t y = e->pos.y.i;
  const uint16_t prev_y = e->prev_pos.y.i;
  yy.i = y;
  yy.d = e->pos.y.d;
  vy = e->vel.y;

  {
    if (y < prev_y) {
      // - collision check (ceil)
      const uint8_t margin_left  = 6;
      const uint8_t margin_right = 6;
      const uint8_t margin_top   = 0;
      const int ty = y + margin_top - 1;
      c1 = mapld_get_object_at(x + margin_left, ty);
      c2 = mapld_get_object_at(x + 15 - margin_right, ty);
      if ((c1 | c2) & 0x80) {
        e->collision |= COLLISION_CEIL;
        yy.i = (y + 15) & 240;
        yy.d = 0;
        vy = abs(e->vel.y);
      }
    }
    else {
      // - collision check (floor)
      const uint8_t margin_left  = 3;
      const uint8_t margin_right = 3;
      const uint8_t margin_bottom = 0;
      const int by = y + 16 - margin_bottom;
      c1 = mapld_get_object_at(x + margin_left, by);
      c2 = mapld_get_object_at(x + 15 - margin_right, by);
      if ((0x7f < c1 && c1 < 0xff) || (0x7f < c2 && c2 < 0xff)) {
        e->collision |= COLLISION_FLOOR;
        yy.i = y & 240;
        yy.d = 0;
        vy = 0;
      }
    }
  }
  {
    // - collision check (left / right)
    const uint8_t margin_left  = 2;
    const uint8_t margin_right = 2;
    const uint8_t margin_top   = 0;
    const uint8_t margin_bottom = 0;

    bool check_right;
    if (prev_x == x) {
      check_right = ((x & 15) < 8);
    }
    else {
      check_right = (prev_x < x);
    }
    uint16_t xa;
    uint16_t xb;
    uint8_t a;
    uint8_t b;
    if (check_right) {
      xa = x + 15 - margin_right;
      xb = ((x + 0) & ~15) + margin_right;
      a = c1;
      b = COLLISION_RIGHT;
    } else {
      xa = x + margin_left;
      xb = ((x + 15) & ~15) - margin_left;
      a = c2;
      b = COLLISION_LEFT;
    }
    const uint8_t o1 = mapld_get_object_at(xa, yy.i + margin_top );
    const uint8_t o2 = mapld_get_object_at(xa, yy.i + 15 - margin_bottom);
    if ((0x7f < o1 && o1 < 0xff) || (0x7f < o2 && o2 < 0xff)) {
      e->pos.x.i = xb;
      e->pos.x.d = 0;
      e->vel.x = 0;
      // recheck ceil/floor, then clear flag or apply y/vy
      if (a & 0x080) {
        e->collision |= b;
      }
      else {
        e->collision = b;
        return;
      }
    }
  }
  e->pos.y.i = yy.i;
  e->pos.y.d = yy.d;
  e->vel.y = vy;
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
  //
  updated = true;
}

static void entity_set_sprites(entity_t * e) {
  if (!e->metasprite) return;
  const int x = e->pos.x.i - camera_get_x();
  const int y = e->pos.y.i - 1;
  sm2_show_sprites(e->plane, e->metasprite, x, y);
}

void entity_update_sprites(void) {
  if (updated) {
    sm2_clear_sprites();
    foreach_active_entity(entity_set_sprites);
    updated = false;
  }
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
