// -*- coding: utf-8-unix -*-

#include "smb1.h"

struct mario_state mario_state;

void mario_init(void) {
  mario_state.input = 0;
  mario_state.speed = 0;
  mario_state.facing = FACING_RIGHT;
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
}

void mario_move(void) {
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

  uint8_t joy = joypad_get_state(1);
  if (joy & FORWARD_KEY) {
    if (joy & VK_FIRE_1) {
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
  if (joy & BACKWARD_KEY) {
    if (brake < mario_state.speed) {
      mario_state.speed -= brake;
    } else {
      mario_state.speed = 0;
    }
  }
  if (0 < mario_state.speed && !(joy & (VK_LEFT | VK_RIGHT))) {
    mario_state.speed -= accel;
  }

  mario_state.input &= 0x30;
  mario_state.input <<= 2;
  mario_state.input |= joy & 0x3f;
}
