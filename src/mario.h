// -*- coding: utf-8-unix -*-

#ifndef MARIO_H
#define MARIO_H

#pragma once

#include <stdlib.h>
#include <msx.h>

#include "types.h"

typedef struct vec2f16q6 {
  f16q6_t x;
  f16q6_t y;
} vec2f16q6_t;

typedef struct vec2f10q6 {
  f10q6_t x;
  f10q6_t y;
} vec2f10q6_t;

typedef struct dynamics_state {
  vec2f16q6_t prev_pos;    // previous position
  vec2f16q6_t pos;         // position
  vec2f10q6_t vel;         // velocity
  vec2f10q6_t acc;         // acceleration
} dynamics_state_t;

void dynamics_state_update(dynamics_state_t* ds);

typedef uint8_t collision_state_t;

enum collision_state {
  COLLISION_CEIL  = (1 << 0),
  COLLISION_FLOOR = (1 << 1),
  COLLISION_LEFT  = (1 << 2),
  COLLISION_RIGHT = (1 << 3),
};

enum mario_pose {
  STANDING    = 0,
  STANDING_L  = 0,
  STANDING_R  = 1,

  JUMPING     = 2,
  JUMPING_L   = 2,
  JUMPING_R   = 3,

  WALKING_1   = 4,
  WALKING_1_L = 4,
  WALKING_1_R = 5,

  WALKING_2   = 6,
  WALKING_2_L = 6,
  WALKING_2_R = 7,

  WALKING_3   = 8,
  WALKING_3_L = 8,
  WALKING_3_R = 9,

  DEAD        = 10,
};

struct mario_state {
  uint8_t input;
  f10q6_t speed;
  enum facing {
    FACING_LEFT  = 0,
    FACING_RIGHT = 1,
  } facing;
  dynamics_state_t dynamics_state;
  collision_state_t collision_state;
};

extern struct mario_state mario_state;

void mario_init(void);
void mario_animate(void);
void mario_move(void);

int16_t mario_get_prev_x(void);
int16_t mario_get_x(void);
void mario_set_x(int16_t x);

#endif
