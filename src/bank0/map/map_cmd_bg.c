// -*- coding: utf-8-unix -*-

#pragma codeseg BANK0

#include "map_cmd.h"
#include "map_img.h"

inline
void map_cmd_bg_mountain(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  uint8_t h = SIZED_OBJECT_LEN(obj);
  uint8_t * p = &(canvas[xy]);
  switch (h) {
  case 3:
    p[2] = 0x49;
    p += ROWS;
    p[1] = 0x49;
    p[2] = 0x59;
    p += ROWS;
    p[0] = 0x3a;
    p[1] = 0x59;
    p[2] = 0x4a;
    p += ROWS;
    p[1] = 0x4b;
    p[2] = 0x5b;
    p += ROWS;
    p[2] = 0x4b;
    break;
  case 2:
    p[1] = 0x49;
    p += ROWS;
    p[0] = 0x3a;
    p[1] = 0x59;
    p += ROWS;
    p[1] = 0x4b;
    break;
  case 1:
    p[0] = 0x3a;
    break;
  default:
    break;
  }
}

inline
void map_cmd_bg_cloud(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  uint8_t w = SIZED_OBJECT_LEN(obj);
  uint8_t * p = &(canvas[xy]);
  p[0] = 0x3d;
  p[1] = 0x4d;
  p += ROWS;
  while (2 < w) {
    p[0] = 0x3e;
    p[1] = 0x4e;
    p += ROWS;
    w--;
  }
  p[0] = 0x3f;
  p[1] = 0x4f;
}

inline
void map_cmd_bg_grass(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  uint8_t w = SIZED_OBJECT_LEN(obj);
  uint8_t * p = &(canvas[xy]);
  p[0] = 0x5d;
  p += ROWS;
  while (2 < w) {
    p[0] = 0x5e;
    p += ROWS;
    w--;
  }
  p[0] = 0x5f;
}

void map_cmd_bg(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  // obj &= ~NEWPAGE;
  const uint8_t id = SIZED_OBJECT_ID(obj);
  switch (id) {
  case ID_MOUNTAIN:
    map_cmd_bg_mountain(xy, obj, canvas);
    return;
  case ID_CLOUD:
    map_cmd_bg_cloud(xy, obj, canvas);
    return;
  case ID_GRASS:
    map_cmd_bg_grass(xy, obj, canvas);
    return;
  default:
    return;
  }
}
