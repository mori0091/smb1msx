// -*- coding: utf-8-unix -*-

#ifndef GLOBALS_H
#define GLOBALS_H

#pragma once

#include <stdlib.h>
#include <msx.h>

/* Tick counter (TIME) of MSX-BASIC */
static volatile __at (0xfc9e) uint16_t JIFFY;

/* timer tick counter */
extern uint16_t tick;

#endif
