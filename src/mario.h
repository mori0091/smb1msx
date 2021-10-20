// -*- coding: utf-8-unix -*-

#ifndef MARIO_H
#define MARIO_H

#pragma once

#include <stdlib.h>
#include <msx.h>

#include "types.h"

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
};

extern struct mario_state mario_state;

void mario_init(void);
void mario_animate(void);
void mario_move(void);

#endif
