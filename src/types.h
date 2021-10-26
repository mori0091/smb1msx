// -*- coding: utf-8-unix -*-

#ifndef TYPES_H
#define TYPES_H

#pragma once

#include <msx.h>

/**
 * Position and size of a rectangle.
 */
typedef struct rect {
  vec2i_t pos;
  vec2i_t size;
} rect_t;

/**
 * Type of 16 bit signed Q6 fixed point number.
 * - integer part : 10 bits
 * - decimal part : 6 bits
 */
typedef int16_t f10q6_t;

/**
 * Constructs a f10q6_t value from integer.
 * \param i   an integer, that shall be 10 bits signed integer.
 */
#define f10q6i(i)    ((f10q6_t)((i) << 6))

/**
 * Constructs a f10q6_t value from floating number.
 * \param f   a floating number.
 */
#define f10q6(f)     ((f10q6_t)((f) * (1 << 6)))

/**
 * 2D vector of f10q6_t
 */
typedef struct vec2f10q6 {
  f10q6_t x;
  f10q6_t y;
} vec2f10q6_t;

/**
 * Type of 22 bit signed fixed point number.
 * - integer part : 16 bits
 * - decimal part : 6 bits
 */
typedef struct f16q6_t {
  int16_t i;                    /* integer part */
  f10q6_t d;                    /* decimal part */
} f16q6_t;

/**
 * 2D vector of f16q6_t
 */
typedef struct vec2f16q6 {
  f16q6_t x;
  f16q6_t y;
} vec2f16q6_t;

#endif
