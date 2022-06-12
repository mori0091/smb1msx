// -*- coding: utf-8-unix -*-

#include <sound.h>

#include "sound_macro.h"

// tick = 60Hz * 60s * 4 / (Tempo * L)
//      = 14400 / (Tempo * L)

// T288
// 1 O4 V15 @0 L4 [4 >cccr8c8 r8cr8cc    <bbbr8b8  r8br8bb]
// 2 O4 V15 @0 L4 [4 fffd8f8  r8fd8f8d8f eeec8e8   r8ec8e8c8e]
// 3 O4 V15 @0 L4 [4 drar8>d8 r2<a>d     <crgr8>c8 r2<g>c<]

static const uint8_t intro_ch1[] = { // Txxx L4 V15 @1
  0x80, 0x0d, V(15), E(1), O5C,      // O5 C4
  0x40, 0x0c, O5C,                   //    C4
  0x40, 0x0d, O5C,                   //    C4
  0x20, 0x06, R,                     //    R8
  0x40, 0x06, O5C,                   //    C8

  0x20, 0x06, R,                     //    R8
  0x40, 0x0d, O5C,                   //    C4
  0x20, 0x06, R,                     //    R8
  0x40, 0x0d, O5C,                   //    C4
  0x40, 0x0c, O5C,                   //    C4

  0x40, 0x0d, O4B,                   // O4 B4
  0x40, 0x0c, O4B,                   //    B4
  0x40, 0x0d, O4B,                   //    B4
  0x20, 0x06, R,                     //    R8
  0x40, 0x06, O4B,                   //    B8

  0x20, 0x06, R,                     //    R8
  0x40, 0x0d, O4B,                   //    B4
  0x20, 0x06, R,                     //    R8
  0x40, 0x0d, O4B,                   //    B4
  0x40, 0x0c, O4B,                   //    B4

  EOM,
};

static const uint8_t intro_ch2[] = { // Txxx L4 V15 @1
  0x80, 0x0d, V(15), E(1), O4F,      // O4 F4
  0x40, 0x0c, O4F,                   //    F4
  0x40, 0x0d, O4F,                   //    F4
  0x40, 0x06, O4D,                   //    D8
  0x40, 0x06, O4F,                   //    F8

  0x20, 0x06, R,                     //    R8
  0x40, 0x0d, O4F,                   //    F4
  0x40, 0x06, O4D,                   //    D8
  0x40, 0x07, O4F,                   //    F8
  0x40, 0x06, O4D,                   //    D8
  0x40, 0x0c, O4F,                   //    F4

  0x40, 0x0d, O4E,                   //    E4
  0x40, 0x0c, O4E,                   //    E4
  0x40, 0x0d, O4E,                   //    E4
  0x40, 0x06, O4C,                   //    C8
  0x40, 0x06, O4E,                   //    E8

  0x20, 0x06, R,                     //    R8
  0x40, 0x0d, O4E,                   //    E4
  0x40, 0x06, O4C,                   //    C8
  0x40, 0x07, O4E,                   //    E8
  0x40, 0x06, O4C,                   //    C8
  0x40, 0x0c, O4E,                   //    E4

  EOM,
};

// Lowered 2 octaves than reference.
static const uint8_t intro_ch3[] = { // Txxx L4 V14 @1
  0x80, 0x0d, V(14), E(1), O2D,      // O4 D4
  0x20, 0x0c, R,                     //    R4
  0x40, 0x0d, O2A,                   //    A4
  0x20, 0x06, R,                     //    R8
  0x40, 0x06, O3D,                   // O5 D8

  0x20, 0x19, R,                     //    R2
  0x40, 0x0d, O2A,                   // O4 A4
  0x40, 0x0c, O3D,                   // O5 D4

  0x40, 0x0d, O2C,                   // O4 C4
  0x20, 0x0c, R,                     //    R4
  0x40, 0x0d, O2G,                   // O4 G4
  0x20, 0x06, R,                     //    R8
  0x40, 0x06, O3C,                   // O5 C8

  0x20, 0x19, R,                     //    R2
  0x40, 0x0d, O2G,                   // O4 G4
  0x40, 0x0c, O3C,                   // O5 C4

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
  &intro, &intro, &intro, &intro, &intro,
};

const struct sound_clip bgm_starman = {
  .priority = 10,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
