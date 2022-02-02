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
  vec2f16q6_t prev_pos;
  /** Position. */
  vec2f16q6_t pos;
  /** Velocity. */
  vec2f10q6_t vel;
  /** Acceleration. */
  vec2f10q6_t acc;
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

void entity_show_sprite(const entity_t * e);

void entity_hide_sprite(const entity_t * e);

/**
 * Initialize physics / entity module.
 */
void entity_init(void);

/**
 * Add an entity to the list of active entities.
 *
 * If the entity has a meta-sprite, the sprite is displayed by calling
 * entity_update_sprites().
 *
 * \param e  an entity.
 */
void entity_add(entity_t * e);

/**
 * Remove an entity from the list of active entities.
 *
 * If the entity has a meta sprite, this function hides that sprite as well.
 *
 * \param e  an entity.
 */
void entity_remove(entity_t * e);

/**
 * Update all active entities' state.
 *
 * \sa entity_update_input()
 * \sa entity_update_dynamics()
 * \sa entity_update_collision()
 * \sa entity_update_speed()
 * \sa entity_run_post_step()
 */
void entity_update(void);

/**
 * Clear the buffer of the sprites.
 *
 * \note
 * This function just updates the internal sprite table on RAM. To show, hide,
 * or move the sprites, call entity_apply_sprites() after this function.
 */
void entity_hide_sprites(void);

/**
 * Updates the buffer of the sprites of all entities according to their status.
 *
 * \note
 * This function just updates the internal sprite table on RAM. To show, hide,
 * or move the sprites, call entity_apply_sprites() after this function.
 */
void entity_update_sprites(void);

/**
 * Writes the buffer of the sprites to the VRAM.
 */
void entity_apply_sprites(void);

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
