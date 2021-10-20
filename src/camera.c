// -*- coding: utf-8-unix -*-

#include "smb1.h"

struct camera camera;

void camera_init(void) {
  camera.x.i = 0;               /* intefer part of camera position */
  camera.x.d = 0;               /* decimal part of camera position */
  camera.speed = 0;             /* scroll speed ; 10.6 fixedpoint [pix] */
}

void camera_move(void) {
  camera.x.d += camera.speed;
  camera.x.i += (camera.x.d >> 6);
  camera.x.d &= ((1 << 6) - 1);
}

void camera_set_speed(f10q6_t speed) {
  camera.speed = speed;
  if (SCROLL_SPEED_MAX < camera.speed) {
    camera.speed = SCROLL_SPEED_MAX;
  }
}
