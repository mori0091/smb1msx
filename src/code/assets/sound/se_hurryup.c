// -*- coding: utf-8-unix -*-

#include <sound.h>

#include "sound_macro.h"

static const uint8_t intro_ch1[] = {
  // T300 V15 @2
  PACK( 6, V(15), E(2), O4E),   // L8 O4 E
  PACK( 6, O5D),                //    O5 D
  PACK( 6, R),                  //       R
  PACK( 6, O5D),                //    O5 D
  PACK(12, O5D),                // L4 O5 D

  PACK( 6, O4F),                // L8 O4 F
  PACK( 6, O5Ds),               //    O5 D+
  PACK( 6, R),                  //       R
  PACK( 6, O5E),                //    O5 E
  PACK(12, O5E),                // L4 O5 E

  PACK( 6, O4Fs),               // L8 O4F+
  PACK( 6, O5E),                //    O5E
  PACK( 6, R),                  //      R
  PACK( 6, O5E),                //    O5E
  PACK(12, O5E),                // L4 O5E

  PACK(12, O5F),                // L4 O5F
  PACK(24, O5F),                // L2 O5F

  PACK(12, R),                  // L4   R

  EOM,
};

static const uint8_t intro_ch2[] = {
  // T300 V15 @2
  PACK( 6, V(15), E(2), O3E),   // L8 O3 E
  PACK(12, O4Gs),               // L4 O4 G+
  PACK( 6, O4G),                // L8 O4 G
  PACK(12, O4G),                // L4 O4 G

  PACK( 6, O3F),                // L8 O3 F
  PACK(12, O4A),                // L4 O4 A
  PACK( 6, O4A),                // L8 O4 A
  PACK(12, O4A),                // L4 O4 A

  PACK( 6, O3Fs),               // L8 O3 F+
  PACK(12, O4As),               // L4 O4 A+
  PACK( 6, O4A),                // L8 O4 A
  PACK(12, O4A),                // L4 O4 A

  PACK(12, O4B),                // L4 O4 B
  PACK(24, O4B),                // L2 O4 B

  PACK(12, R),                  // L4    R

  EOM,
};

static const uint8_t intro_ch3[] = {
  // T300 V15 @2
  PACK( 6, V(15), E(2), O3B),   // L8  O3 B
  PACK( 6, O4B),                // L8  O4 B
  PACK( 6, R),                  // L8     R
  PACK(18, O4B),                // L4. O4 B

  PACK( 6, O4C),                // L8  O4 C
  PACK( 6, O5C),                // L8  O5 C
  PACK( 6, R),                  // L8     R
  PACK(18, O5C),                // L4. O5 C

  PACK( 6, O4Cs),               // L8  O4 C+
  PACK( 6, O5Cs),               // L8  O5 C+
  PACK( 6, R),                  // L8     R
  PACK(18, O5C),                // L4. O5 C

  PACK(12, O3G),                // L4  O3 G
  PACK(24, O3G),                // L2  O3 G

  PACK(12, R),                  // L4     R

  EOM,
};

static const struct sound_fragment intro = {
  .streams = {
    [0] = intro_ch1,
    [1] = intro_ch2,
    [2] = intro_ch3,
  }
};

static const struct sound_fragment * const music[] = {
  &intro,
};

const struct sound_clip se_hurryup = {
  .priority = 255,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
