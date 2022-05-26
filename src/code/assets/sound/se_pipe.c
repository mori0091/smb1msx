// -*- coding: utf-8-unix -*-

#include <sound.h>

// tick = 60Hz * 60s * 4 / (Tempo * L)
//      = 14400 / (Tempo * L)

// "T300 o4 v15 L48 >[3 [2 AD<G> r16] r12]"
static const uint8_t intro_ch1[] = {
  0x60, 0x01, 0x8f, 0x00, 0x7f, // V15 O5 A48
  0x40, 0x01,       0x00, 0xbe, //     O5 D48
  0x40, 0x01,       0x01, 0x1d, //     O4 G48
  0x20, 0x03,       0x10,       //        R16

  0x40, 0x01,       0x00, 0x7f, //     O5 A48
  0x40, 0x01,       0x00, 0xbe, //     O5 D48
  0x40, 0x01,       0x01, 0x1d, //     O4 G48
  0x20, 0x03,       0x10,       //        R16

  0x20, 0x04,       0x10,       //        R12

  0xff,
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
  &intro,
  &intro,
};

const struct sound_clip se_pipe = {
  .priority = 2,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
