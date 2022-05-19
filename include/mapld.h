// -*- coding: utf-8-unix -*-

#pragma once

#ifndef MAPLD_H
#define MAPLD_H

#include <stdint.h>

#include "macros.h"

/**
 * Initialize the map loader.
 */
void mapld_init(void);

/**
 * Load next half page of the map.
 */
void mapld_load_next_half_page(void);

/**
 * Returns the buffer page number according to the column number of stage map.
 *
 * \param col  the column number
 * \return     the buffer page number
 */
inline uint8_t mapld_get_buffer_page_at(uint16_t col) {
  return (col >> 4) % STAGEMAP_BUFFER_PAGES;
}

/**
 * Returns the pointer to the map buffer element according to the given (row,
 * col) index.
 *
 * \param row, col  (row, column) index
 * \return          pointer to the map buffer element.
 */
uint8_t* mapld_get_buffer_ptr_at(uint8_t row, uint16_t col);

/**
 * Returns an object at the given position.
 *
 * \param x  x-coordinate in the stage. [pix]
 * \param y  y-coordinate in the stage. [pix]
 * \return   the object code at (x,y)
 */
uint8_t mapld_get_object_at(int x, int y);

#endif
