// -*- coding: utf-8-unix -*-

#pragma once

#ifndef STAGE_H
#define STAGE_H

#include <stdint.h>

/**
 * Initialize `stage` module.
 */
void stage_init(void);

/**
 * Setup and render the 1st page of stage map.
 */
void stage_setup(void);

/**
 * Update rendered stage map according to camera's position and speed.
 *
 * This shall be called every frame periodically in main-loop of game engine.
 */
void stage_update(void);

/**
 * Reset stage map and then forward it until catching up to camera's position.
 */
void stage_warp_to_camera_position(void);

/**
 * Replace the tile at the given (row, col).
 *
 * Used to dynamically update objects on the current map.
 *
 * \param row   row of the current map
 * \param col   column of the current map
 * \param tile  Tile ID of the object to replace the one of (row, col).
 */
void stage_put_tile(uint8_t row, uint8_t col, uint8_t tile);

#endif
