// -*- coding: utf-8-unix -*-

#ifndef GEOM_H
#define GEOM_H

#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * Rectanglar shape - a rectangle, border, margin, etc.
 */
typedef struct rect {
  int left;
  int top;
  int right;
  int bottom;
} rect_t;

/**
 * Returns width of the rectangle.
 *
 * \param rect  a rectangle.
 * \return      width of the rectangle.
 */
inline int rect_width(const rect_t * rect) {
  return (rect->right - rect->left + 1);
}

/**
 * Returns height of the rectangle.
 *
 * \param rect  a rectangle.
 * \return      height of the rectangle.
 */
inline int rect_height(const rect_t * rect) {
  return (rect->bottom - rect->top + 1);
}

/**
 * Tests if the rectangle contains the point.
 *
 * \param rect  a rectangle.
 * \param x,y   XY-coordinates of a point.
 * \return      true if the rectangle contains the point, false otherwise.
 */
bool rect_contains_xy(const rect_t * rect, int x, int y);

/**
 * Tests if two rectangles intersects.
 *
 * \param a,b   two rectangles.
 * \return      true if two rectangles intersects, false otherwise.
 */
bool rect_intersects(const rect_t * a, const rect_t * b);

#endif
