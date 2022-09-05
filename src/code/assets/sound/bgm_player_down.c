// -*- coding: utf-8-unix -*-

#include <sound.h>

#include "sound_macro.h"

static const uint8_t intro_ch1[] = {
  // T180 V15
  PACK( 2, V(15), O5C),         // L32 O5 C
  PACK( 3, O5Cs),               // L32    C+
  PACK( 5, O5D),                // L16    D
  PACK(10, R),                  // L8     R
  PACK(20, R),                  // L4     R

  EOM,
};

static const uint8_t intro_ch2[] = {
  // T180 V15
  PACK( 2, V(15), O4C),         // L32 O4 C
  PACK( 3, O4Cs),               // L32    C+
  PACK( 5, O4D),                // L16    D
  PACK(10, R),                  // L8     R
  PACK(20, R),                  // L4     R

  EOM,
};

static const struct sound_fragment intro = {
  .streams = {
    [0] = 0,
    [1] = intro_ch1,
    [2] = intro_ch2,
  }
};

static const uint8_t a_part_ch1[] = {
  // T180 V15
  PACK(10, V(15), O4B),         // L8 O4 B
  PACK(10, O5F),                // L8 O5 F
  PACK(10, R),                  // L8    R
  PACK(10, O5F),                // L8 O5 F
  PACK(14, O5F),                // L6 O5 F
  PACK(13, O5E),                // L6 O5 E
  PACK(13, O5D),                // L6 O5 D
  PACK(20, O5C),                // L4 O5 C
  PACK(20, R),                  // L4    R
  PACK(40, R),                  // L2    R

  EOM,
};

static const uint8_t a_part_ch2[] = {
  // T180 V15
  PACK(20, V(15), O4G),         // L4 O4 G
  PACK(10, R),                  // L8    R
  PACK(10, O4G),                // L8 O4 G
  PACK(14, O4G),                // L6 O4 G
  PACK(13, O4A),                // L6 O4 A
  PACK(13, O4B),                // L6 O4 B
  PACK(20, O5C),                // L4 O5 C
  PACK(20, O4G),                // L4 O4 G
  PACK(20, O4C),                // L4 O4 C
  PACK(20, R),                  // L4    R

  EOM,
};

static const uint8_t a_part_ch3[] = {
  // T180 V15
  PACK(20, V(15), O3G),         // L4 O3 G
  PACK(10, R),                  // L8    R
  PACK(10, O3G),                // L8 O3 G
  PACK(14, O3G),                // L6 O3 G
  PACK(13, O3A),                // L6 O3 A
  PACK(13, O3B),                // L6 O3 B
  PACK(20, O4C),                // L4 O4 C
  PACK(20, O3G),                // L4 O3 G
  PACK(20, O3C),                // L4 O3 C
  PACK(20, R),                  // L4    R

  EOM,
};

static const struct sound_fragment a_part = {
  .streams = {
    [0] = a_part_ch1,
    [1] = a_part_ch2,
    [2] = a_part_ch3,
  }
};

static const struct sound_fragment * const music[] = {
  &intro,
  &a_part,
};

const struct sound_clip bgm_player_down = {
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
