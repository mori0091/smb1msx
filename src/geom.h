// -*- coding: utf-8-unix -*-

#ifndef GEOM_H
#define GEOM_H

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
 * Tests if the rectangle contains the point.
 *
 * \param rect  a rectangle.
 * \param x,y   XY-coordinates of a point.
 * \return      true if the rectangle contains the point, false otherwise.
 */
bool rect_contains_xy(const rect_t * rect, int x, int y);

/**
 * Tests if the rectangle contains the point.
 *
 * \param rect  a rectangle.
 * \param pt    a point.
 * \return      true if the rectangle contains the point, false otherwise.
 */
inline
bool rect_contains_pt(const rect_t * rect, const vec2i_t * pt) {
  return rect_contains_xy(rect, pt->x, pt->y);
}

/**
 * Tests if two rectangles intersects.
 *
 * \param a,b   two rectangles.
 * \return      true if two rectangles intersects, false otherwise.
 */
bool rect_intersects(const rect_t * a, const rect_t * b);

#endif
