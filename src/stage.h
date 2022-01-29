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
void stage_setup(void);

/**
 * Update rendered stage map according to camera's position and speed.
 *
 * This shall be called every frame periodically in main-loop of game engine.
 */
void stage_update(void);

#endif
