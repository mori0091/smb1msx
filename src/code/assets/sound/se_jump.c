// -*- coding: utf-8-unix -*-

#include <sound.h>

#include "sound_macro.h"

// 1 T225 L64 O4 V15 >AFD<B AA+BB (>CC+D+ (EEFF+ (GG+AA+ (BB>CC+ (DD+EE (FF+GG+ (A
// 2 T225 L64 O3 V15 >AFD<B AA+BB (>CC+D+ (EEFF+ (GG+AA+ (BB>CC+ (DD+EE (FF+GG+ (A

static const uint8_t intro_ch1[] = {
  // T225 L64
  PACK( 1, V(15), O5A),         // V15 O5 A
  PACK( 1, O5F),                //     O5 F
  PACK( 1, O5D),                //     O5 D
  PACK( 1, O4B),                //     O4 B
  PACK( 1, O4A),                //     O4 A
  PACK( 1, O4As),               //     O4 A+
  PACK( 1, O4B),                //     O4 B
  PACK( 1, O4B),                //     O4 B
  PACK( 1, V(14), O5C),         // V14 O5 C
  PACK( 1, O5Cs),               //     O5 C+
  PACK( 1, O5D),                //     O5 D
  PACK( 1, O5Ds),               //     O5 D+
  PACK( 1, V(13), O5E),         // V13 O5 E
  PACK( 1, O5E),                //     O5 E
  PACK( 1, O5F),                //     O5 F
  PACK( 1, O5Fs),               //     O5 F+
  PACK( 1, V(12), O5G),         // V12 O5 G
  PACK( 1, O5Gs),               //     O5 G+
  PACK( 1, O5A),                //     O5 A
  PACK( 1, O5As),               //     O5 A+
  PACK( 1, V(11), O5B),         // V11 O5 B
  PACK( 1, O5B),                //     O5 B
  PACK( 1, O6C),                //     O6 C
  PACK( 1, O6Cs),               //     O6 C+
  PACK( 1, V(10), O6D),         // V10 O6 D
  PACK( 1, O6Ds),               //     O6 D+
  PACK( 1, O6E),                //     O6 E
  PACK( 1, O6E),                //     O6 E
  PACK( 1, V(9), O6F),          // V9  O6 F
  PACK( 1, O6Fs),               //     O6 F+
  PACK( 1, O6G),                //     O6 G
  PACK( 1, O6Gs),               //     O6 G+
  PACK( 1, V(8), O6A),          // V8  O6 A

  EOM,
};

static const uint8_t intro_ch2[] = {
  // T225 L64
  PACK( 1, V(15), O4A),         // V15 O4 A
  PACK( 1, O4F),                //     O4 F
  PACK( 1, O4D),                //     O4 D
  PACK( 1, O3B),                //     O3 B
  PACK( 1, O3A),                //     O3 A
  PACK( 1, O3As),               //     O3 A+
  PACK( 1, O3B),                //     O3 B
  PACK( 1, O3B),                //     O3 B
  PACK( 1, V(14), O4C),         // V14 O4 C
  PACK( 1, O4Cs),               //     O4 C+
  PACK( 1, O4D),                //     O4 D
  PACK( 1, O4Ds),               //     O4 D+
  PACK( 1, V(13), O4E),         // V13 O4 E
  PACK( 1, O4E),                //     O4 E
  PACK( 1, O4F),                //     O4 F
  PACK( 1, O4Fs),               //     O4 F+
  PACK( 1, V(12), O4G),         // V12 O4 G
  PACK( 1, O4Gs),               //     O4 G+
  PACK( 1, O4A),                //     O4 A
  PACK( 1, O4As),               //     O4 A+
  PACK( 1, V(11), O4B),         // V11 O4 B
  PACK( 1, O4B),                //     O4 B
  PACK( 1, O5C),                //     O5 C
  PACK( 1, O5Cs),               //     O5 C+
  PACK( 1, V(10), O5D),         // V10 O5 D
  PACK( 1, O5Ds),               //     O5 D+
  PACK( 1, O5E),                //     O5 E
  PACK( 1, O5E),                //     O5 E
  PACK( 1, V(9), O5F),          // V9  O5 F
  PACK( 1, O5Fs),               //     O5 F+
  PACK( 1, O5G),                //     O5 G
  PACK( 1, O5Gs),               //     O5 G+
  PACK( 1, V(8), O5A),          // V8  O5 A

  EOM,
};

static const struct sound_fragment intro = {
  .streams = {
    [0] = 0,
    [1] = intro_ch1,
    [2] = intro_ch2,
  },
};

static const struct sound_fragment * const music[] = {
  &intro,
};

const struct sound_clip se_jump = {
  .priority = 0,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
