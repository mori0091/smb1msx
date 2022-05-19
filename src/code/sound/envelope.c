// -*- coding: utf-8-unix -*-

#include <sound_eg.h>

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

static const uint8_t ev3[] = {
  0xff,0xcf,0xff,0xcf,
  0xef,0xbf,0xef,0xbf,
  0xdf,0xaf,0xdf,0xaf,
  0xcf,0x9f,0xcf,0x9f,
  0xbf,0x8f,0xbf,0x8f,
};
static const struct sound_eg_LPCM envelope_3 = {
  .len = sizeof(ev3),
  .data = ev3,
};

const struct sound_eg_attribute envelope_table[SOUND_EG_TABLE_MAX] = {
  [0] = {.clz = SOUND_EG_AHDSR, .param = &envelope_0},
  [1] = {.clz = SOUND_EG_AHDSR, .param = &envelope_1},
  [2] = {.clz = SOUND_EG_AHDSR, .param = &envelope_2},
  [3] = {.clz = SOUND_EG_LPCM , .param = &envelope_3},
};
