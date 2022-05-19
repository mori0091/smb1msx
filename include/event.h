// -*- coding: utf-8-unix -*-

#pragma once

#ifndef EVENT_H
#define EVENT_H

enum event {
  EV_NONE = 0,
  EV_PLAYER_DIES,
  EV_PLAYER_ENTERS_LOWER_PIPE,
  EV_PLAYER_EXITS_LOWER_PIPE,
  EV_PLAYER_ENTERS_RIGHT_PIPE,
};

void event_init(void);

enum event event_get(void);

void event_set(const enum event ev);

#endif
