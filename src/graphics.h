// -*- coding: utf-8-unix -*-

#ifndef GRAPHICS_H
#define GRAPHICS_H

#pragma once

#include <stdint.h>

void graphics_init_vdp(void);

void graphics_clear_vram(void);

void graphics_hide_sprite(uint8_t plane);

void graphics_hide_all_sprites(void);

#define graphics_clear_sprite_pat(pat, n_bytes) \
  graphics_fill_sprite_pat((pat), 0, (n_bytes))

void graphics_fill_sprite_pat(uint8_t pat, uint8_t x, uint8_t n_bytes);

void graphics_set_sprite_pat(uint8_t pat, const uint8_t* p, uint8_t n_bytes);

void graphics_set_sprite(uint8_t plane, const struct sprite* s);

#endif
