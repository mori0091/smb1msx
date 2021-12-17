// -*- coding: utf-8-unix -*-

#include "../smb1.h"

static const struct sound_eg_AHDSR envelope_0 = {
  .attack_rate = 255,
  .hold_time = 0,
  .decay_rate = 8,
  .sustain_level = 0,
  .release_rate = 255,
};

static const struct sound_eg_AHDSR envelope_1 = {
  .attack_rate = 255,
  .hold_time = 0,
  .decay_rate = 10,
  .sustain_level = 64,
  .release_rate = 32,
};

static const struct sound_eg_AHDSR envelope_2 = {
  .attack_rate = 128,
  .hold_time = 0,
  .decay_rate = 4,
  .sustain_level = 32,
  .release_rate = 16,
};

const struct sound_eg_attribute envelope_table[SOUND_EG_TABLE_MAX] = {
  [0] = {.clz = SOUND_EG_AHDSR, .param = &envelope_0},
  [1] = {.clz = SOUND_EG_AHDSR, .param = &envelope_1},
  [2] = {.clz = SOUND_EG_AHDSR, .param = &envelope_2},
};
