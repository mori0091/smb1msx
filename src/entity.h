// -*- coding: utf-8-unix -*-

#ifndef ENTITY_H
#define ENTITY_H

#pragma once

#include <stdint.h>
#include "geom.h"
#include "physics.h"

#define ITEM_NONE          (0)
#define ITEM_COIN          (1)
#define ITEM_MUSHROOM      (2)
#define ITEM_1UP_MUSHROOM  (3)
#define ITEM_FIREFLOWER    (4)
#define ITEM_STARMAN       (5)

#define TILE_EMPTY         (0x7f)
#define TILE_BLOCK         (0xb4)   // #14 (#112, #116)
#define TILE_BRICK         (0xd1)   // #15 (#120, #124)

void entity_get_bounds(const entity_t * e, rect_t * rect);

void physics_add_block_entity(uint8_t row, uint8_t col, uint8_t item, uint8_t tile);

#endif
