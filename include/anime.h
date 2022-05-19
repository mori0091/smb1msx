// -*- coding: utf-8-unix -*-

#pragma once

#ifndef ANIME_H
#define ANIME_H

#include <stdbool.h>

void anime_set_enable_on_vsync(bool enable);

void anime_on_vsync(void);

void anime_update(void);

#endif
