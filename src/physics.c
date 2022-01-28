// -*- coding: utf-8-unix -*-

#include "smb1.h"

entity_t entities[8];
entity_t * const player = &entities[0];

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

  e->pos.x.d += e->vel.x;
  e->pos.x.i += e->pos.x.d >> 6;
  e->pos.x.d &= ((1 << 6) - 1);

  e->pos.y.d += e->vel.y;
  e->pos.y.i += e->pos.y.d >> 6;
  e->pos.y.d &= ((1 << 6) - 1);

  e->vel.x += e->acc.x;
  e->vel.y += e->acc.y;

  /* correct y-axis position and velocity */
  if (e->vel.y < f10q6i(-16)) {
    e->vel.y = f10q6i(-16);
  }
  if (f10q6i(16) < e->vel.y) {
    e->vel.y = f10q6i(16);
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
  static uint8_t c1;            // ceil / floor object #1
  static uint8_t c2;            // ceil / floor object #2
  static f16q6_t yy;            // Candidate for corrected Y-coordinate position.
  static f10q6_t vy;            // Candidate for corrected Y-coordinate velocity.

  c1 = 0;
  c2 = 0;
  e->collision = 0;
  if (e->pos.y.i <= -16) {
    return;
  }

  const uint16_t x0 = e->pos.x.i;
  yy.i = e->pos.y.i;
  yy.d = e->pos.y.d;
  vy = e->vel.y;

  {
    if (e->pos.y.i < e->prev_pos.y.i) {
      // - collision check (ceil)
      static const rect_t box = {
        .pos  = { 6, 0 },
        .size = { 4,16 },
      };
      const int lx = x0 + box.pos.x;
      const int rx = lx + box.size.x - 1;
      const int ty = e->pos.y.i + box.pos.y - 1;
      c1 = mapld_get_object_at(lx, ty);
      c2 = mapld_get_object_at(rx, ty);
      if ((c1 | c2) & 0x80) {
        e->collision |= COLLISION_CEIL;
        yy.i = (e->pos.y.i + 15) & 240;
        yy.d = 0;
        vy = abs(e->vel.y);
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
      const int by = e->pos.y.i + box.pos.y + box.size.y;
      c1 = mapld_get_object_at(lx, by);
      c2 = mapld_get_object_at(rx, by);
      if ((c1 | c2) & 0x80) {
        e->collision |= COLLISION_FLOOR;
        yy.i = e->pos.y.i & 240;
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
    if (e->prev_pos.x.i == e->pos.x.i) {
      check_right = (e->pos.x.i & 15) < 8;
    } else {
      check_right = e->prev_pos.x.i < e->pos.x.i;
    }

    uint16_t xa;
    uint16_t xb;
    uint8_t a;
    uint8_t b;
    if (check_right) {
      xa = x0 + box.pos.x + box.size.x - 1;
      xb = ((e->pos.x.i +  0) & 0x0fff0) - box.pos.x - box.size.x + 16;
      a = c1;
      b = COLLISION_RIGHT;
    } else {
      xa = x0 + box.pos.x;
      xb = ((e->pos.x.i + 15) & 0x0fff0) - box.pos.x;
      a = c2;
      b = COLLISION_LEFT;
    }
    const uint8_t obj = (mapld_get_object_at(xa, yy.i + 0 ) |
                         mapld_get_object_at(xa, yy.i + 15));
    if (obj & 0x080) {
      e->pos.x.i = xb;
      e->pos.x.d = 0;
      e->vel.x = 0;
      e->collision |= b;
      // recheck ceil/floor, then clear flag or apply y/vy
      if (!(a & 0x080)) {
        e->collision &= ~(COLLISION_FLOOR | COLLISION_CEIL);
        return;
      }
    }
  }
  e->pos.y.i = yy.i;
  e->pos.y.d = yy.d;
  e->vel.y = vy;
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
  f10q6_t speed = abs(e->vel.x);

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

void entity_update(void) {
  // Action planning task
  entity_update_input(player);
  // Dynamics state update task (position)
  entity_update_dynamics(player);
  player_correct_position();
  // Collision test task
  entity_update_collision(player);
  // Dynamics state update task (velocity / acceleration)
  entity_update_speed(player);
  // Pose update & Sound / Visual effects, etc.
  entity_run_post_step(player);
}
