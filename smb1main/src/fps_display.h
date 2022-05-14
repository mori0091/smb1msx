// -*- coding: utf-8-unix -*-

#ifndef FPS_DISPLAY_H
#define FPS_DISPLAY_H

#pragma once

#include <stdbool.h>

void fps_display_set_visible(bool visible);
void fps_display_reset(void);
void fps_display_update(void);

#endif
