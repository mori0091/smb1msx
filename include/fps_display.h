// -*- coding: utf-8-unix -*-

#pragma once

#ifndef FPS_DISPLAY_H
#define FPS_DISPLAY_H

#include <stdbool.h>

void fps_display_set_visible(bool visible);
void fps_display_reset(void);
void fps_display_update(void);

#endif
