// -*- coding: utf-8-unix -*-

#ifndef PHYSICS_H
#define PHYSICS_H

#pragma once

#include <stdlib.h>
#include <msx.h>

#include "types.h"

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

typedef struct entity entity_t;

typedef void (* entity_callback_t)(entity_t * e);

typedef uint8_t (* controller_t)(void);

typedef uint8_t input_state_t;

typedef uint8_t collision_state_t;

enum collision_state {
  COLLISION_CEIL  = (1 << 0),
  COLLISION_FLOOR = (1 << 1),
  COLLISION_LEFT  = (1 << 2),
  COLLISION_RIGHT = (1 << 3),
};

enum facing {
  FACING_LEFT  = 0,
  FACING_RIGHT = 1,
};

struct entity {
  controller_t controller;
  input_state_t input;
  vec2f16q6_t prev_pos;    // previous position
  vec2f16q6_t pos;         // position
  vec2f10q6_t vel;         // velocity
  vec2f10q6_t acc;         // acceleration
  collision_state_t collision;
  entity_callback_t post_step;
  enum facing facing;
};

extern entity_t * const player;

inline int16_t player_get_prev_x(void) {
  return player->prev_pos.x.i;
}

inline int16_t player_get_x(void) {
  return player->pos.x.i;
}

inline void player_set_x(int16_t x) {
  player->pos.x.i = x;
}

inline f10q6_t player_get_speed(void) {
  return abs(player->vel.x);
}

/**
 * A controller function that always return 0 (no input).
 * \return 0
 * \sa entity_set_controller()
 */
uint8_t no_controller(void);

/**
 * A controller function that return the state of joystick #1.
 * \return state of joystick #1
 * \sa entity_set_controller()
 */
uint8_t joystick1(void);

void entity_set_controller(entity_t * e, controller_t c);

inline void entity_set_post_step(entity_t * e, entity_callback_t cb) {
  e->post_step = cb;
}

inline enum facing entity_get_facing(entity_t * e) {
  return e->facing;
}

inline void entity_set_facing(entity_t * e, enum facing lr) {
  e->facing = lr;
}

/**
 * Update all live entities' state.
 *
 * \sa entity_update_input()
 * \sa entity_update_dynamics()
 * \sa entity_update_collision()
 * \sa entity_update_speed()
 * \sa entity_run_post_step()
 */
void entity_update(void);

/**
 * Action planning task.
 *
 * \param e  an entity.
 */
void entity_update_input(entity_t * e);

/**
 * Dynamics state update task (position).
 *
 * \param e  an entity.
 */
void entity_update_dynamics(entity_t * e);

/**
 * Collision test task.
 *
 * \param e  an entity.
 */
void entity_update_collision(entity_t * e);

/**
 * Dynamics state update task (velocity / acceleration).
 *
 * \param e  an entity.
 */
void entity_update_speed(entity_t * e);

/**
 * Pose update & Sound / Visual effects, etc.
 *
 * \param e  an entity.
 * \sa entity_set_post_step()
 */
void entity_run_post_step(entity_t * e);

#endif
