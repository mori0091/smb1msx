// -*- coding: utf-8-unix -*-

#pragma once

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

/**
 * Type of 16 bit signed Q8 fixed point number.
 * - integer part : 8 bits
 * - decimal part : 8 bits
 */
typedef int16_t f8q8_t;

/**
 * Constructs a f8q8_t value from integer.
 * \param i   an integer, that shall be 10 bits signed integer.
 */
#define f8q8i(i)    ((f8q8_t)((i) << 8))

/**
 * Constructs a f8q8_t value from floating number.
 * \param f   a floating number.
 */
#define f8q8(f)     ((f8q8_t)((f) * (1 << 8)))

/**
 * 2D vector of f8q8_t
 */
typedef struct vec2f8q8 {
  f8q8_t x;
  f8q8_t y;
} vec2f8q8_t;

/**
 * Type of 24 bit signed fixed point number.
 * - integer part : 16 bits
 * - decimal part : 8 bits
 */
typedef struct f16q8_t {
  int16_t i;                    /* integer part */
  uint8_t d;                    /* decimal part */
} f16q8_t;

/**
 * 2D vector of f16q8_t
 */
typedef struct vec2f16q8 {
  f16q8_t x;
  f16q8_t y;
} vec2f16q8_t;

#endif
