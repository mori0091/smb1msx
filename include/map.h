// -*- coding: utf-8-unix -*-

#pragma once

#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#include "macros.h"

/**
 * Initialize the map loader.
 */
void map_init(void);

/**
 * Load next half page of the map.
 */
void map_load_next_half_page(void);

/**
 * Returns the buffer page number according to the column number of stage map.
 *
 * \param col  the column number
 * \return     the buffer page number
 */
inline uint8_t map_get_buffer_page_at(uint16_t col) {
  return (col >> 4) % STAGEMAP_BUFFER_PAGES;
}

/**
 * Returns the pointer to the map buffer element according to the given (row,
 * col) index.
 *
 * \param row, col  (row, column) index
 * \return          pointer to the map buffer element.
 */
uint8_t* map_get_buffer_ptr_at(uint8_t row, uint16_t col);

/**
 * Replace an object tile at the given (row, col) index.
 *
 * \param row, col  (row, column) index
 * \param tile      the object tile code
 */
void map_put_tile(uint8_t row, uint8_t col, uint8_t tile);

/**
 * Update an attribute at the given (row, col) index.
 *
 * \param row, col  (row, column) index
 * \note This shall be called when an item was poped out.
 */
void map_consume_item(uint8_t row, uint8_t col);

/**
 * Returns an object tile at the given position.
 *
 * \param x  x-coordinate in the stage. [pix]
 * \param y  y-coordinate in the stage. [pix]
 * \return   the object tile code at (x,y)
 */
uint8_t map_get_object_at(int x, int y);

/**
 * Returns an attribute at the given position.
 *
 * \param x  x-coordinate in the stage. [pix]
 * \param y  y-coordinate in the stage. [pix]
 * \return   the attribute code at (x,y)
 */
uint8_t map_get_attribute_at(int x, int y);

#endif
