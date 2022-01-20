// -*- coding: utf-8-unix -*-

#ifndef PHYSICS_H
#define PHYSICS_H

#pragma once

#include <stdlib.h>
#include <msx.h>

#include "types.h"

typedef struct dynamics_state {
  vec2f16q6_t prev_pos;    // previous position
  vec2f16q6_t pos;         // position
  vec2f10q6_t vel;         // velocity
  vec2f10q6_t acc;         // acceleration
} dynamics_state_t;

typedef uint8_t collision_state_t;

enum collision_state {
  COLLISION_CEIL  = (1 << 0),
  COLLISION_FLOOR = (1 << 1),
  COLLISION_LEFT  = (1 << 2),
  COLLISION_RIGHT = (1 << 3),
};

void dynamics_state_update(dynamics_state_t* ds);

void collision_state_update(collision_state_t* cs, dynamics_state_t* ds);

#endif
