// -*- coding: utf-8-unix -*-

#ifndef SM2_H
#define SM2_H

#pragma once

#include <stdint.h>
#include <metasprite.h>

/**
 * Clear reservations to show / hide sprites.
 */
void sm2_clear_sprites(void);

/**
 * Reserves to hide sprites.
 *
 * \param plane  the starting plane number.
 * \param ms     a metasprite to be hidden.
 */
void sm2_hide_sprites(uint8_t plane, const metasprite_t * ms);

/**
 * Reserves to show sprites.
 *
 * \param plane  the starting plane number.
 * \param ms     a metasprite to be shown.
 */
void sm2_show_sprites(uint8_t plane, const metasprite_t * ms, int x, int y);

/**
 * Apply a show/hide request for the currently reserved sprite to sprite
 * attribute table on VRAM.
 */
void sm2_apply_sprites(void);

/**
 * Return whether the buffer is dirty.
 *
 * The module is assumed to have a flag **dirty** and this function returns the
 * state of the **dirty** flag.
 *
 * If the internal buffer of the module has been modified and not yet applied to
 * VRAM, the buffer is assumed to be dirty.
 *
 * The **dirty** flag is set to `true` when sm2_hide_sprites() or
 * sm2_show_sprites() was called, and is reset to `false` when
 * sm2_clear_sprites() or sm2_apply_sprites() was called.
 *
 * \return  whether the buffer is dirty.
 */
bool sm2_is_dirty(void);

#endif
