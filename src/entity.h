// -*- coding: utf-8-unix -*-

#ifndef ENTITY_H
#define ENTITY_H

#pragma once

#include <stdint.h>

#define ITEM_NONE          (0)
#define ITEM_COIN          (1)
#define ITEM_MUSHROOM      (2)
#define ITEM_1UP_MUSHROOM  (3)
#define ITEM_FIREFLOWER    (4)
#define ITEM_STARMAN       (5)

void entity_add_block(uint8_t row, uint8_t col, uint8_t item);

void entity_add_brick(uint8_t row, uint8_t col, uint8_t item);

#endif
