// -*- coding: utf-8-unix -*-

#include <sound.h>

#include "sound_macro.h"

static const uint8_t intro_ch1[] = {
  // T225 L64
  PACK( 1, V(15), O3A),
  PACK( 1, O3F),
  PACK( 1, O3D),
  PACK( 1, O2B),
  PACK( 1, O2A),
  PACK( 1, O2As),
  PACK( 1, O2B),
  PACK( 1, O2B),

  EOM,
};

static const struct sound_fragment intro = {
  .streams = {
    [0] = 0,
    [1] = intro_ch1,
    [2] = 0,
  },
};

static const struct sound_fragment * const music[] = {
  &intro,
};

const struct sound_clip se_block = {
  .priority = 1,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
