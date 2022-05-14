// -*- coding: utf-8-unix -*-

#include "smb1.h"

uint16_t text_x;
uint16_t text_y;
uint8_t text_fg_color;
uint8_t text_bg_color;

static uint8_t buf[4];
static uint8_t cc[4];

void text_putc(uint8_t c) {
  cc[0] = (text_bg_color << 4) | text_bg_color;
  cc[1] = (text_bg_color << 4) | text_fg_color;
  cc[2] = (text_fg_color << 4) | text_bg_color;
  cc[3] = (text_fg_color << 4) | text_fg_color;

  vmemptr_t dst = IMAGE + (uint32_t)128 * text_y + text_x / 2;
  const uint8_t* src = font_get_8x8_gryph(c);
  for (int i = 0; i < 8; ++i) {
    const uint8_t bits = *src;
    buf[0] = cc[(bits >> 6) & 3];
    buf[1] = cc[(bits >> 4) & 3];
    buf[2] = cc[(bits >> 2) & 3];
    buf[3] = cc[(bits     ) & 3];
    vdp_cmd_await();
    vmem_write(dst, buf, 4);
    dst += 128;
    src ++;
  }
  locate(text_x + 8, text_y);
}

void text_puts(const uint8_t* s) {
  const uint16_t x = text_x;
  uint8_t c;
  while ((c = *s++)) {
    if (c == '\n') {
      locate(x, text_y + 8);
    } else {
      text_putc(c);
    }
  }
}
