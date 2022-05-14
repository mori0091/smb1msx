// -*- coding: utf-8-unix -*-

#include "geom.h"

bool rect_contains_xy(const rect_t * rect, int x, int y) {
  return ((rect->left <= x) && (x <= rect->right) &&
          (rect->top  <= y) && (y <= rect->bottom));
}

bool rect_intersects(const rect_t * a, const rect_t * b) {
  rect_t r;
  r.left   = b->left - rect_width(a);
  r.top    = b->top  - rect_height(a);
  r.right  = b->right;
  r.bottom = b->bottom;
  return rect_contains_xy(&r, a->left, a->top);
}
