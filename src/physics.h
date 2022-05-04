// -*- coding: utf-8-unix -*-

#ifndef PHYSICS_H
#define PHYSICS_H

#pragma once

#include <stdlib.h>
#include <msx.h>

#include "entity.h"

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

inline f8q8_t player_get_speed(void) {
  return abs(player->vel.x);
}

/**
 * Initialize physics / entity module.
 */
void physics_init(void);

/**
 * Add an entity to the list of active entities.
 *
 * If the entity has a meta-sprite, the sprite is displayed by calling
 * entity_update_sprites().
 *
 * \param e  an entity.
 */
void physics_add_entity(entity_t * e);

/**
 * Remove an entity from the list of active entities.
 *
 * If the entity has a meta sprite, this function hides that sprite as well.
 *
 * \param e  an entity.
 */
void physics_remove_entity(entity_t * e);

/**
 * Update all active entities' state.
 *
 * \sa entity_update_input()
 * \sa entity_update_dynamics()
 * \sa entity_update_collision()
 * \sa entity_update_speed()
 * \sa entity_run_post_step()
 */
void physics_update(void);

/**
 * Updates the buffer of the sprites of all entities according to their status.
 *
 * \note
 * This function just updates the internal sprite table on RAM. To show, hide,
 * or move the sprites, call entity_apply_sprites() after this function.
 */
void physics_update_sprites(void);

/**
 * Writes the buffer of the sprites to the VRAM.
 */
void physics_apply_sprites(void);

#endif
