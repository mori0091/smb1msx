// -*- coding: utf-8-unix -*-

#pragma codeseg BANK0

#include "event.h"

static enum event evt;

void event_init(void) {
  evt = EV_NONE;
}

enum event event_get(void) {
  return evt;
}

void event_set(const enum event ev) {
  evt = ev;
}
