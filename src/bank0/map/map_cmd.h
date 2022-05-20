// -*- coding: utf-8-unix -*-

#pragma once

#ifndef MAP_CMD_H
#define MAP_CMD_H

#include <stdint.h>

#include "macros.h"

// opcode

/** End of stream. (1 byte code) */
#define EOS                     (0x0ff)

/** Construct Row and Column index. (The first byte of 2 bytes code) */
#define XY(x, y)                (uint8_t)((((x) & 15) << 4) | ((y) & 15))

/** Extract Y-component of Row and Column index. */
#define ROW(xy)                 ((xy) & 15)

/** Extract X-component of Row and Column index. */
#define COL(xy)                 (((xy) >> 4) & 15)

// operand

/**
 * Page delimiter bit.
 *
 * If the NEWPAGE bit of 2nd byte is set, the command will be the first command
 * on the next page. Therefore, the NEWPAGE bit must not be set for the command
 * on page #0.
 */
#define NEWPAGE                 (uint8_t)(0x80)

/**
 * Constract bytecode of an object w/ length.
 *
 * \param id   object id (1..7)
 * \param len  width and/or height of the object (1..16)
 * \return     bytecode of an object w/ length
 */
#define SIZED_OBJECT(id, len)   (uint8_t)((((id) & 7) << 4) | (((len) - 1) & 15))

/**
 * Extract ID-component of a sized object.
 *
 * \param obj  an object constructed by SIZED_OBJECT()
 * \return     object id (1..7)
 */
#define SIZED_OBJECT_ID(obj)    (((obj) >> 4) & 7)

/**
 * Extract LENGTH-component of a sized object.
 *
 * \param obj  an object constructed by SIZED_OBJECT()
 * \return     length (1..16)
 */
#define SIZED_OBJECT_LEN(obj)   (((obj) & 15) + 1)

// ---- BG (landscape) layer ----

#define ID_MOUNTAIN             (1)
#define ID_CLOUD                (2)
#define ID_GRASS                (3)

#define MOUNTAIN(height)        SIZED_OBJECT(ID_MOUNTAIN, (height))
#define CLOUD(width)            SIZED_OBJECT(ID_CLOUD, (width))
#define GRASS(width)            SIZED_OBJECT(ID_GRASS, (width))

enum map_bg {
  MAP_BG_OVERWORLD_FINE_DAY = 0,
  MAP_BG_OVERWORLD_ATHLETIC,
  MAP_BG_OVER_SKY,
  MAP_BG_UNDER_WATER,
  MAP_BG_UNDERWORLD,
  MAP_BG_CASTLE,
};

/**
 * BG layer table.
 *
 * The list of pointers to map command streams for BG layer.
 * The list is indexed by `enum map_bg`.
 */
extern const uint8_t * const map_bg_layers[];

/**
 * Put the tile-Ids of a BG object to the page buffer.
 *
 * \param xy      Row and Column index    (the 1st byte of a map command)
 * \param obj     bytecode of a BG object (the 2nd byte of a map command)
 * \param canvas  the page buffer
 */
void map_bg(uint8_t xy, uint8_t obj, uint8_t * canvas);


// ---- FG (object) layer ----

// (0 <= y <= 12)

#define Q_BLOCK_POWERUP          (0x00)
#define Q_BLOCK_COIN             (0x01)

// #define HIDDEN_POWERUP           (0x--)
#define HIDDEN_COIN              (0x02)
#define HIDDEN_1UP               (0x03)

#define BRICK_POWERUP            (0x04)
#define BRICK_VINE_PLANT         (0x05)
#define BRICK_STARMAN            (0x06)
#define BRICK_10_COINS           (0x07)
#define BRICK_1UP                (0x08)
//#define BRICK_COIN               (0x--)

#define H_PIPE_HEAD              (0x09)

#define FIXED_BLOCK              (0x0a)

#define SPRING_BOARD             (0x0b)

#define INV_L_SHAPED_PIPE        (0x0c)

#define FLAG_POLE                (0x0d)

#define H_COINS(width)           SIZED_OBJECT(2, (width))
#define H_BLOCKS(width)          SIZED_OBJECT(3, (width))
#define V_BLOCKS(width)          SIZED_OBJECT(4, (width))
#define H_BRICKS(width)          SIZED_OBJECT(5, (width))
#define V_BRICKS(width)          SIZED_OBJECT(6, (width))
#define V_PIPE(height)           SIZED_OBJECT(7, (height))
#define V_PIPE_ENTER(height)     SIZED_OBJECT(7, 8+(height))

// (y = 13)

#define UP_STAIRS(height)        SIZED_OBJECT(1, height)
#define DOWN_STAIRS(height)      SIZED_OBJECT(2, height)
#define CASTLE(height)           SIZED_OBJECT(3, height)

// (y = 14)

#define VALLEY(width)            SIZED_OBJECT(1, width)

/**
 * FG layer table.
 *
 * The list of pointers to map command streams for FG layer.
 */
extern const uint8_t * const map_fg_layers[];

/**
 * Put the tile-Ids of an object to the page buffer.
 *
 * \param xy      Row and Column index  (the 1st byte of a map command)
 * \param obj     bytecode of an object (the 2nd byte of a map command)
 * \param canvas  the page buffer
 */
void map_fg(uint8_t xy, uint8_t obj, uint8_t * canvas);

#endif
