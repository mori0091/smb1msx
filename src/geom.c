// -*- coding: utf-8-unix -*-

#include "geom.h"

bool rect_contains_xy(const rect_t * rect, int x, int y) {
  int px = x - rect->pos.x;
  int py = y - rect->pos.y;
  return ((0 <= px) && (px < rect->size.x) &&
          (0 <= py) && (py < rect->size.y));
}

bool rect_intersects(const rect_t * a, const rect_t * b) {
  rect_t r;
  r.pos.x = b->pos.x - a->size.x;
  r.pos.y = b->pos.y - a->size.y;
  r.size.x = b->size.x + a->size.x;
  r.size.y = b->size.y + a->size.y;
  return rect_contains_pt(&r, &a->pos);
}
