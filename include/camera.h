// -*- coding: utf-8-unix -*-

#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "smb1.h"

/* map position (camera position) */
struct camera {
  f16q8_t x;
  f8q8_t speed;
};

extern struct camera camera;

void camera_init(void);

void camera_update(void);

inline f8q8_t camera_get_speed(void) {
  return camera.speed;
}

void camera_set_speed(f8q8_t speed);

inline int16_t camera_get_x(void) {
  return camera.x.i;
}

inline void camera_set_x(int16_t x) {
  camera.x.i = x;
}

#endif
