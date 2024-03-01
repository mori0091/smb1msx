// -*- coding: utf-8-unix -*-

#pragma codeseg BANK1

#include <vdp.h>
#include <vmem.h>

#include "text.h"
#include "font.h"
#include "macros.h"

uint16_t text_x;
uint16_t text_y;
static uint8_t text_fg_color;
static uint8_t text_bg_color;

static uint8_t buf[4];
static uint8_t cc[4];

void set_foreground_color(uint8_t fg) {
  set_text_color(fg, text_bg_color);
}

void set_background_color(uint8_t bg) {
  set_text_color(text_fg_color, bg);
}

void set_text_color(uint8_t fg, uint8_t bg) {
  text_fg_color = fg;
  text_bg_color = bg;
  cc[0] = (bg << 4) | bg;
  cc[1] = (bg << 4) | fg;
  cc[2] = (fg << 4) | bg;
  cc[3] = (fg << 4) | fg;
}

static void text_put_char(uint8_t c) {
  vmemptr_t dst = IMAGE + (uint32_t)128 * text_y + text_x / 2;
  const uint8_t* src = font_get_8x8_gryph(c);
  for (int i = 0; i < 8; ++i) {
    const uint8_t bits = *src;
    buf[0] = cc[(bits >> 6) & 3];
    buf[1] = cc[(bits >> 4) & 3];
    buf[2] = cc[(bits >> 2) & 3];
    buf[3] = cc[(bits     ) & 3];
    vmem_write(dst, buf, 4);
    dst += 128;
    src++;
  }
  locate(text_x + 8, text_y);
}

void text_putc(uint8_t c) {
  vdp_cmd_await();
  text_put_char(c);
}

void text_puts(const uint8_t* s) {
  const uint16_t x = text_x;
  uint8_t c;
  vdp_cmd_await();
  while ((c = *s++)) {
    if (c == '\n') {
      locate(x, text_y + 8);
    } else {
      text_put_char(c);
    }
  }
}
