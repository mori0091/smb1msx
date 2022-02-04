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

#endif
