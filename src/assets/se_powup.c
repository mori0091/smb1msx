// -*- coding: utf-8-unix -*-

#include <msx.h>

// "T225 L32 C<G>CEG>C<G <A->CE-A-E-A->CE-A- <<B->DFB-FB->DFB-F<"
static const uint8_t intro_ch1[] = { // T225 L32 V15
  0x60, 0x02, 0x8f, 0x01, 0xac,      // O4 C
  0x40, 0x02, 0x02, 0x3b,            // O3 G
  0x40, 0x02, 0x01, 0xac,            // O4 C
  0x40, 0x02, 0x01, 0x53,            //    E
  0x40, 0x02, 0x01, 0x1d,            //    G
  0x40, 0x02, 0x00, 0xd6,            // O5 C
  0x40, 0x02, 0x01, 0x1d,            // O4 G

  0x40, 0x02, 0x02, 0x1b,            // O3 A- (G+)
  0x40, 0x02, 0x01, 0xac,            // O4 C
  0x40, 0x02, 0x01, 0x68,            //    E- (D+)
  0x40, 0x02, 0x01, 0x0d,            //    A- (G+)
  0x40, 0x02, 0x01, 0x68,            //    E- (D+)
  0x40, 0x02, 0x01, 0x0d,            //    A- (G+)
  0x40, 0x02, 0x00, 0xd6,            // O5 C
  0x40, 0x02, 0x00, 0xb4,            //    E- (D+)
  0x40, 0x02, 0x00, 0x87,            //    A- (G+)

  0x40, 0x02, 0x01, 0xe0,            // O3 B- (A+)
  0x40, 0x02, 0x01, 0x7d,            // O4 D
  0x40, 0x02, 0x01, 0x40,            //    F
  0x40, 0x02, 0x00, 0xf0,            //    B- (A+)
  0x40, 0x02, 0x01, 0x40,            //    F
  0x40, 0x02, 0x00, 0xf0,            //    B- (A+)
  0x40, 0x02, 0x00, 0xbe,            // O5 D
  0x40, 0x02, 0x00, 0xa0,            //    F
  0x40, 0x02, 0x00, 0x78,            //    B- (A+)
  0x40, 0x02, 0x00, 0xa0,            //    F

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
};

const struct sound_clip se_powup = {
  .priority = 2,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
