// -*- coding: utf-8-unix -*-

#ifndef STAGE_H
#define STAGE_H

#pragma once

#include <stdint.h>

/**
 * Initialize `stage` module.
 */
void stage_init(void);

/**
 * Setup and render the 1st page of stage map.
 */
void stage_setup_map(void);

/**
 * Returns number of columns of the current stage.
 *
 * \return number of columns of the current stage.
 */
uint16_t stage_get_columns(void);

/**
 * Returns number of horizontal pixels of the current stage.
 *
 * \return TILE_WIDTH * stage_get_columns()
 */
uint16_t stage_get_width(void);

/**
 * Returns an object at the given position.
 *
 * \param x  x-coordinate in the stage. [pix]
 * \param y  y-coordinate in the stage. [pix]
 * \return   the object code at (x,y)
 */
uint8_t stage_get_object_at(int x, int y);

/**
 * Update rendered stage map according to camera's position and speed.
 *
 * This shall be called every frame periodically in main-loop of game engine.
 */
void stage_update_map(void);

#endif
