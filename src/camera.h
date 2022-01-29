// -*- coding: utf-8-unix -*-

#ifndef CAMERA_H
#define CAMERA_H

#pragma once

#include "smb1.h"

/* map position (camera position) */
struct camera {
  f16q6_t x;
  f10q6_t speed;
};

extern struct camera camera;

void camera_init(void);

void camera_update(void);

inline f10q6_t camera_get_speed(void) {
  return camera.speed;
}

void camera_set_speed(f10q6_t speed);

inline int16_t camera_get_x(void) {
  return camera.x.i;
}

inline void camera_set_x(int16_t x) {
  camera.x.i = x;
}

#endif
