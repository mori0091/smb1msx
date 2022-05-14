// -*- coding: utf-8-unix -*-

#ifndef ANIME_H
#define ANIME_H

#pragma once

#include <stdbool.h>

void anime_set_enable_on_vsync(bool enable);

void anime_on_vsync(void);

void anime_update(void);

#endif
