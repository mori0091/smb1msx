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

struct mario_state mario_state;

const vec2i_t W16H32D2[] = {
  {0,0},                        // layer #1 (lower body #1)
  {0,0},                        // layer #2 (lower body #2)
  {0,-16},                      // layer #3 (upper body #1)
  {0,-16},                      // layer #4 (upper body #2)
};

const uint8_t mario_pats[] = {
  0,                            // layer #1 (lower body #1)
  4,                            // layer #2 (lower body #2)
  8,                            // layer #3 (upper body #1)
  12,                           // layer #4 (upper body #2)
};

const metasprite_t mario_metasprite = {
  .n = 4,
  .anchor = {0,0},
  .layouts = W16H32D2,
  .pats = mario_pats,
};

void mario_init(void) {
  // mario_state.life = 3;
  mario_state.input = 0;
  mario_state.speed = 0;
  mario_set_facing(FACING_RIGHT);
  mario_set_pose(STANDING);
  mario_reset_physical_status();
  mario_reset_ability();

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

#define MARIO_PAT1_DX  (X_FROM_VMEMPTR(SPRITE_PATTERNS))
#define MARIO_PAT1_DY  (Y_FROM_VMEMPTR(SPRITE_PATTERNS))
#define MARIO_PAT2_DX  (MARIO_PAT1_DX+128)
#define MARIO_PAT2_DY  (MARIO_PAT1_DY)

#define BLANK_PAT_SX   (X_FROM_VMEMPTR(REF_SPRITE_PATTERNS)+128)
#define BLANK_PAT_SY   (Y_FROM_VMEMPTR(REF_SPRITE_PATTERNS)+255)

#define SPRITE_PAT_SX(idx)                                  \
  (X_FROM_VMEMPTR(REF_SPRITE_PATTERNS) + ((idx) & 1) * 128)

#define SPRITE_PAT_SY(idx)                                  \
  (Y_FROM_VMEMPTR(REF_SPRITE_PATTERNS) + ((idx) / 2))

static void mario_set_sprite_pat(uint8_t idx) {
  if (mario_has_super_ability()) {
    // layer #0, #1 (lower body)
    vdp_cmd_execute_HMMM(SPRITE_PAT_SX(idx),
                         SPRITE_PAT_SY(idx),
                         128, 1,
                         MARIO_PAT1_DX,
                         MARIO_PAT1_DY);
    // layer #2, #3 (upper body)
    vdp_cmd_execute_HMMM(SPRITE_PAT_SX(idx),
                         SPRITE_PAT_SY(idx),
                         128, 1,
                         MARIO_PAT2_DX,
                         MARIO_PAT2_DY);
  }
  else {
    // layer #0, #1 (lower body)
    vdp_cmd_execute_HMMM(SPRITE_PAT_SX(idx),
                         SPRITE_PAT_SY(idx),
                         128, 1,
                         MARIO_PAT1_DX,
                         MARIO_PAT1_DY);
    // layer #2, #3 (upper body ; blank)
    vdp_cmd_execute_HMMM(BLANK_PAT_SX,
                         BLANK_PAT_SY,
                         128, 1,
                         MARIO_PAT2_DX,
                         MARIO_PAT2_DY);
  }
}

void mario_show(int x, int y) {
  vmem_set_metasprite_a(SPRITES_0, 0, x, y, &mario_metasprite);
}

static uint8_t anim_tick;

void mario_animate(void) {
  uint8_t idx = mario_state.pose + mario_state.facing;
  if (mario_state.pose != WALKING) {
    mario_set_sprite_pat(idx);
    return;
  }
  /* pose == WALKING */
  anim_tick++;
  if (speed_lo < mario_state.speed) {
    anim_tick++;
  }
  if (6 <= anim_tick) {
    anim_tick = 0;
  }
  const uint8_t t = anim_tick & ~1;
  mario_set_sprite_pat(t + idx);
}

void mario_animate_die(void) {
  mario_set_pose(DEAD);
  mario_set_facing(0);
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

  await_vsync();
  mario_animate();
  mario_move_sprite();
  sleep_millis(500);
  do {
    tick++;
    await_vsync();
    // mario_animate();
    mario_move_sprite();
    if (tick & 1) continue;
    dynamics_state_update(&mario_state.dynamics_state);
    if (mario_state.dynamics_state.pos.y.i < -32) {
      mario_state.dynamics_state.pos.y.i = -32;
      mario_state.dynamics_state.pos.y.d = 0;
    }
  } while (mario_state.dynamics_state.pos.y.i <= 240);
}

uint8_t no_controller(void) {
  return 0;
}

uint8_t joystick1(void) {
  return joypad_get_state(1);
}

static controller_t controller = joystick1;

void mario_set_controller(controller_t c) {
  if (c) {
    controller = c;
  }
  else {
    controller = no_controller;
  }
}

void mario_update_input_state(void) {
  mario_state.input &= (VK_FIRE_0 | VK_FIRE_1);
  mario_state.input <<= 2;
  mario_state.input |= controller();
}

static void mario_update_speed_on_floor(void) {
  {
    if (mario_state.dynamics_state.vel.x < 0) {
      mario_set_facing(FACING_LEFT);
    }
    if (0 < mario_state.dynamics_state.vel.x) {
      mario_set_facing(FACING_RIGHT);
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
      mario_set_facing(FACING_LEFT);
    }
    if (LR_KEY == VK_RIGHT) {
      mario_set_facing(FACING_RIGHT);
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
      mario_set_pose(JUMPING);
      // ---- sound effect ----
      sound_effect(&se_jump);
    }
    // estimate mario's pose
    else if (mario_state.speed == 0) {
      mario_set_pose(STANDING);
    }
    else {
      mario_set_pose(WALKING);
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
      mario_state.dynamics_state.vel.x = 0;
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
