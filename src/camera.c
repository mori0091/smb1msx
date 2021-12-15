// -*- coding: utf-8-unix -*-

#include "smb1.h"

struct camera camera;

void camera_init(void) {
  camera.x.i = 0;               /* intefer part of camera position */
  camera.x.d = 0;               /* decimal part of camera position */
  camera.speed = 0;             /* scroll speed ; 10.6 fixedpoint [pix] */
}

// /* auto scroll */
// void camera_move(void) {
//   camera.x.d += camera.speed;
//   camera.x.i += (camera.x.d >> 6);
//   camera.x.d &= ((1 << 6) - 1);
// }

/* follow mario */
void camera_move(void) {
  const int16_t sx = camera_get_x();
  const int16_t x0 = mario_get_prev_x();
  const int16_t x = mario_get_x();
  const int16_t u = x - x0;
  if (x <= sx + 50 || u <= 0) {
    camera_set_speed(0);
  }
  else if (sx + 124 <= x) {
    camera.x.i = (x - 124 + sx + u) / 2;
    camera.x.d = mario_state.dynamics_state.pos.x.d;
    camera_set_speed(mario_state.speed);
  }
  else if (u <= 4) {
    camera.x.i++;
    camera_set_speed(f10q6i(1));
  }
  else {
    camera.x.i += u - 4;
    camera_set_speed(f10q6i(u - 4));
  }
}

void camera_set_speed(f10q6_t speed) {
  camera.speed = speed;
  if (SCROLL_SPEED_MAX < camera.speed) {
    camera.speed = SCROLL_SPEED_MAX;
  }
}
