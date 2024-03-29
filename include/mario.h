// -*- coding: utf-8-unix -*-

#pragma once

#ifndef MARIO_H
#define MARIO_H

#include "macros.h"
#include "assets.h"
#include "event.h"
#include "physics.h"
#include "entity.h"

enum mario_pose {
  STANDING    = 0,
  JUMPING     = 1,
  WALKING     = 2,
  WALKING_0   = 2,
  WALKING_1   = 3,
  WALKING_2   = 4,
  BRAKING     = 5,
  CLIMBING    = 6,
  CLIMBING_0  = 6,
  CLIMBING_1  = 7,
  SWIMING     = 8,
  SWIMING_0   = 8,
  SWIMING_1   = 9,
  SWIMING_2   = 10,
  SWIMING_3   = 11,
  SWIMING_4   = 12,
  SWIMING_5   = 13,             /* only for super mario */
  CROUCHING   = 14,             /* only for super mario */
  DEAD        = 15,             /* only for mario */
  TRANFORMING = 15,             /* only for super mario */
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
  uint8_t coin;
  int8_t life;
  enum mario_pose pose;
  uint8_t status;
  uint16_t status_timer;     // countdown timer for INVINCIBLE / WEAKENED status
};

extern struct mario_state mario_state;

void mario_init(void);
void mario_animate(void);
void mario_animate_die(void);

void mario_show(int x, int y);

inline void mario_set_pose(enum mario_pose pose) {
  mario_state.pose = pose;
}

inline uint8_t mario_get_ability(void) {
  return mario_state.status & ABILITY_MASK;
}

inline void mario_reset_ability(void) {
  mario_state.status &= ~ABILITY_MASK;
  vdp_set_palette(1, color_palette[1]);
  vdp_set_palette(2, color_palette[2]);
  vdp_set_palette(3, color_palette[3]);
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
  vdp_set_palette(1, color_palette[14]);
  vdp_set_palette(2, color_palette[13]);
  vdp_set_palette(3, color_palette[15]);
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
  if (event_get() != EV_PLAYER_DIES && (mario_state.status & STATUS_INVINCIBLE)) {
    sound_set_bgm(&bgm_over_world);
    sound_start();                // start BGM
  }
  mario_state.status &= ~PHYSICAL_STATUS_MASK;
  if (mario_has_fire_ability()) {
    vdp_set_palette(1, color_palette[14]);
    vdp_set_palette(2, color_palette[13]);
    vdp_set_palette(3, color_palette[15]);
  }
  else {
    vdp_set_palette(1, color_palette[1]);
    vdp_set_palette(2, color_palette[2]);
    vdp_set_palette(3, color_palette[3]);
  }
}

inline void mario_set_physical_status(enum physical_status status) {
  mario_reset_physical_status();
  mario_state.status |= status;
  if (status) {
    mario_state.status_timer = msx_get_vsync_frequency() * 12; /* 12sec */
  }
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
    mario_set_physical_status(STATUS_WEAKENED);
  }
  else {
    event_set(EV_PLAYER_DIES);
  }
  mario_reset_ability();
}

inline int8_t mario_get_life(void) {
  return mario_state.life;
}

inline void mario_set_life(int8_t life) {
  mario_state.life = life;
}

inline void mario_died(void) {
  mario_state.life--;
  mario_reset_physical_status();
  mario_reset_ability();
}

inline void mario_1up(void) {
  mario_state.life++;
}

inline bool mario_is_over(void) {
  return mario_state.life <= 0;
}

#endif
