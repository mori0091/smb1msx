// -*- coding: utf-8-unix -*-

#include "smb1.h"

/* default color palette */
const palette_t color_palette[16] = {
  RGB(0,0,0), RGB(6,1,0), RGB(3,3,1), RGB(7,5,4),
  RGB(4,6,0), RGB(0,5,0), RGB(1,5,7), RGB(7,4,1),
  RGB(6,2,0), RGB(1,1,1), RGB(7,5,5), RGB(6,2,0),
  RGB(4,5,7), RGB(6,1,0), RGB(7,7,7), RGB(7,5,0),
};

/* timer tick counter */
uint16_t tick;
