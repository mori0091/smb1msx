// -*- coding: utf-8-unix -*-

#ifndef MARIO_H
#define MARIO_H

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

  WALKING     = 4,
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

// data SuperAbility   = Normal | SuperPower
// data FireAbility    = Normal | FirePower
// data PhysicalStatus = Normal | Invincible | Weakened
// type PlayerStatus   = (Physical, SuperAbility, FireAbility)
#define PHYSICAL_STATUS_MASK (0x03)
#define ABILITY_MASK         (SUPER_ABILITY_MASK | FIRE_ABILITY_MASK)
#define SUPER_ABILITY_MASK   (0x04)
#define FIRE_ABILITY_MASK    (0x08)

enum physical_status {
  STATUS_NORMAL     = 0,
  STATUS_INVINCIBLE = 1,
  STATUS_WEAKENED   = 2,
};

struct mario_state {
  int8_t life;
  uint8_t input;
  f10q6_t speed;
  enum facing {
    FACING_LEFT  = 0,
    FACING_RIGHT = 1,
  } facing;
  enum mario_pose pose;
  uint8_t status;
  uint8_t status_timer;         // countdown timer for INVINCIBLE / WEAKENED status
  dynamics_state_t dynamics_state;
  collision_state_t collision_state;
};

extern struct mario_state mario_state;

void mario_init(void);
void mario_animate(void);
void mario_animate_die(void);
void mario_backup_input_state(void);
void mario_update_input_state(void);
void mario_move(void);

int16_t mario_get_prev_x(void);
int16_t mario_get_x(void);
void mario_set_x(int16_t x);

inline void mario_set_pose(enum mario_pose pose) {
  mario_state.pose = pose;
}

inline void mario_set_facing(enum facing facing) {
  mario_state.facing = facing;
}

inline enum physical_status mario_get_physical_status(void) {
  return mario_state.status & PHYSICAL_STATUS_MASK;
}

inline bool mario_is_invincible(void) {
  return (mario_get_physical_status() == STATUS_INVINCIBLE);
}

inline bool mario_is_weakened(void) {
  return (mario_get_physical_status() == STATUS_WEAKENED);
}

inline void mario_reset_physical_status(void) {
  mario_state.status &= ~PHYSICAL_STATUS_MASK;
}

inline void mario_set_physical_status(enum physical_status status) {
  mario_reset_physical_status();
  mario_state.status |= status;
  if (status) {
    // \TODO
    // mario_set_status_countdown_timer(3*30); // 3 seconds
  }
}

inline uint8_t mario_get_ability(void) {
  return mario_state.status & ABILITY_MASK;
}

inline void mario_reset_ability(void) {
  mario_state.status &= ~ABILITY_MASK;
}

inline void mario_set_ability(uint8_t ability) {
  mario_reset_ability();
  mario_state.status |= ability & ABILITY_MASK;
}

inline bool mario_has_super_ability(void) {
  return mario_state.status & SUPER_ABILITY_MASK;
}

inline void mario_enable_super_ability(void) {
  mario_state.status |= SUPER_ABILITY_MASK;
}

inline bool mario_has_fire_ability(void) {
  return mario_state.status & FIRE_ABILITY_MASK;
}

inline void mario_enable_fire_ability(void) {
  mario_state.status |= FIRE_ABILITY_MASK;
}

inline void mario_power_up(void) {
  if (mario_has_fire_ability()) {
    // \TODO bonus point
    return;
  }
  if (mario_has_super_ability()) {
    // \TODO Fire powered animation
    mario_enable_fire_ability();
    return;
  }
  {
    // \TODO Super powered animation
    mario_enable_super_ability();
  }
}

inline void mario_power_down(void) {
  if (mario_has_super_ability()) {
    // \TODO Power down animation
    mario_reset_ability();
    mario_set_physical_status(STATUS_WEAKENED);
  }
  else {
    event_set(EV_PLAYER_DIES);
  }
}

inline int8_t mario_get_life(void) {
  return mario_state.life;
}

inline void mario_set_life(int8_t life) {
  mario_state.life = life;
}

inline void mario_died(void) {
  mario_state.life--;
}

inline void mario_1up(void) {
  mario_state.life++;
}

inline bool mario_is_over(void) {
  return mario_state.life <= 0;
}

#endif
