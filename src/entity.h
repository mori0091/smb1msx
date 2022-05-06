// -*- coding: utf-8-unix -*-

#ifndef ENTITY_H
#define ENTITY_H

#pragma once

#include <stdint.h>
#include "geom.h"
#include "types.h"

#define A_BUTTON       VK_FIRE_0
#define B_BUTTON       VK_FIRE_1
#define PREV_A_BUTTON  (VK_FIRE_0 << 2)
#define PREV_B_BUTTON  (VK_FIRE_1 << 2)

#define speed_hi       f8q8i(6)
#define speed_lo       f8q8(3.6)
#define accel_hi       ((uint8_t)80)
#define accel          ((uint8_t)40)
#define brake          ((uint8_t)96)
#define initial_vy_hi  f8q8(-8.25)
#define initial_vy_lo  f8q8(-8.0)
#define gravity_hi     f8q8(2.0)
#define gravity_lo     f8q8(0.5)

// | entity | entity # | plane # |
// | ------ | -------- | ------- |
// | N/A    | N/A      | #0..#1  | ... reserved for fps display
// | player | 0        | #2..#5  |
// | blocks | 1        | #6..#7  |
// | items  | 2        | #8..#9  |

#define PLANE_BLOCKS       (6)
#define PLANE_ITEMS        (8)

#define ITEM_NONE          (0)
#define ITEM_COIN          (1)
#define ITEM_MUSHROOM      (2)
#define ITEM_1UP_MUSHROOM  (3)
#define ITEM_FIREFLOWER    (4)
#define ITEM_STARMAN       (5)

#define TILE_EMPTY         (0x7f)
#define TILE_BLOCK         (0xb4)   // #14 (#112, #116)
#define TILE_BRICK         (0xd1)   // #15 (#120, #124)

extern const vec2i_t W16H16D2[];

/**
 * Extra state / properties for items and blocks.
 */
typedef struct entity_state {
  uint8_t tile;                 /* for block / brick */
  uint8_t item;
  uint8_t tick;
  uint8_t row0, col0;
  uint16_t x0, y0;
} entity_state_t;

typedef struct entity entity_t;

typedef void (* entity_callback_t)(entity_t * e);

typedef uint8_t (* controller_t)(void);

enum collision {
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
  /** Controller function to generate input signals. */
  controller_t controller;
  /** Input signals. */
  uint8_t input;
  /** Previous position. */
  vec2f16q8_t prev_pos;
  /** Position. */
  vec2f16q8_t pos;
  /** Velocity. */
  vec2f8q8_t vel;
  /** Acceleration. */
  vec2f8q8_t acc;
  /** Collision flags. */
  uint8_t collision;
  /** Objects that the entity collided with. */
  uint8_t c1, c2;
  /** Post-step function. */
  entity_callback_t post_step;
  /** The direction in which the entity is facing. */
  enum facing facing;
  /** Metasprite of this entity (optional). */
  const metasprite_t * metasprite;
  // /** Sprite palette number. */
  // enum sprite_palette sp_palette;
  /** The first plane number of the sprite. */
  uint8_t plane;
};

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

/**
 * Set / unset the entity's controller function.
 *
 * \param e   the entity.
 * \param c   a controller function.
 */
void entity_set_controller(entity_t * e, controller_t c);

/**
 * Set / unset the entity's post-step callback function.
 *
 * \param e   the entity.
 * \param cb  a post-step callback function.
 */
inline void entity_set_post_step(entity_t * e, entity_callback_t cb) {
  e->post_step = cb;
}

/**
 * Get the direction in which the entity is facing.
 *
 * \param e   the entity.
 * \return    the direction (FACING_LEFT or FACING_RIGHT)
 */
inline enum facing entity_get_facing(entity_t * e) {
  return e->facing;
}

/**
 * Set the direction in which the entity is facing.
 *
 * \param e   the entity.
 * \param lr  the direction (FACING_LEFT or FACING_RIGHT)
 */
inline void entity_set_facing(entity_t * e, enum facing lr) {
  e->facing = lr;
}

/**
 * Set the entity's meta-sprite.
 *
 * \param ms   meta-sprite.
 */
inline void entity_set_metasprite(entity_t * e, const metasprite_t * ms) {
  e->metasprite = ms;
}

// /**
//  * Set the entity's sprite palette number.
//  *
//  * \param sp_palette   sprite palette number.
//  *
//  * \sa assets_set_sprite_palette()
//  */
// inline void entity_set_sprite_palette(entity_t * e, enum sprite_palette sp_palette) {
//   e->sp_palette = sp_palette;
// }

void entity_get_bounds(const entity_t * e, rect_t * rect);

// -----------

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

// -----------

void mushroom_entity_new(uint8_t row, uint8_t col, uint8_t item);

void block_entity_new(uint8_t row, uint8_t col, uint8_t item, uint8_t tile);

#endif
