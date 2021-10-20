// -*- coding: utf-8-unix -*-

#ifndef TYPES_H
#define TYPES_H

#pragma once

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
 * Type of 22 bit signed fixed point number.
 * - integer part : 16 bits
 * - decimal part : 6 bits
 */
typedef struct f16q6_t {
  int16_t i;                    /* integer part */
  f10q6_t d;                    /* decimal part */
} f16q6_t;

#endif
