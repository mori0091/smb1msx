// -*- coding: utf-8-unix -*-

#ifndef SM2_H
#define SM2_H

#pragma once

#include <stdint.h>
#include <metasprite.h>

void sm2_init(void);

void sm2_add_sprites(uint8_t plane, const metasprite_t * ms, int x, int y);

void sm2_apply_sprites(void);

#endif
