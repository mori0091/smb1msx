// -*- coding: utf-8-unix -*-

#ifndef SOUND_MACRO_H_
#define SOUND_MACRO_H_

#include <stdint.h>

// utility
#define VARIADIC_SIZE(...)                                               \
  VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55,   \
                  54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42,    \
                  41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29,    \
                  28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,    \
                  15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, )
#define VARIADIC_SIZE_I(                                                 \
    e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14,     \
    e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27,     \
    e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40,     \
    e41, e42, e43, e44, d45, e46, e47, e48, e49, e50, e51, e52, e53,     \
    e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, size, ...)         \
  size

// -----------------------------------------------------------------------

/**
 * Pack duration and commands into a chunk.
 *
 * \param tick   Duration expressed in tick counts at 60 Hz.
 * \param ...    A series of commands (except for EOM) to pack into a chunk.
 *
 * \note
 * The number of octets in the command is automatically calculated and encoded
 * into a chunk along with the tick count and command.
 */
#define PACK(tick, ...)                                    \
  PACK0(tick, VARIADIC_SIZE(__VA_ARGS__), __VA_ARGS__)
#define PACK0(tick, n, ...)                                \
  (uint8_t)((((n) << 5) & 0xe0) + (((tick) >> 8) & 0x1f)), \
  (uint8_t)((tick) & 255),                                 \
  __VA_ARGS__

// commands --------------------------------------------------------------

// End of music track / section
#define EOM       (0xff)

// Key on (Tone FDR 0..0x0fff)
#define KEYON(x)  (uint8_t)(0x00 + (((x) >> 8) & 15)), (uint8_t)((x) & 255)

// Change Tone (Tone FDR 0..0x0fff)
#define TONE(x)   (uint8_t)(0xe0 + (((x) >> 8) & 15)), (uint8_t)((x) & 255)

// Key off
#define KEYOFF()  (0x10)

// Noise 0..31
#define N(x)      (uint8_t)(0x20 + (x))

// MIXer 0x00..0x3f (000000b .. 111111b)
#define MIX(x)    (0x70), (uint8_t)(x)

// Volume 0..15
#define V(x)      (uint8_t)(0x80 + (x))

// Software Envelope # 0..15
#define E(x)      (uint8_t)(0xc0 + (x))

// Hardware Envelope # 0..15
#define S(x)      (uint8_t)(0x90 + (x))

// Hardware Envelope FDR 0..65535
#define M(y)      (0x40),                (uint8_t)(((y) >> 8) & 255), ((y) & 255)

// Hardware Envelope # and FDR
#define SM(x,y)   (uint8_t)(0xd0 + (x)), (uint8_t)(((y) >> 8) & 255), ((y) & 255)

// Rest
#define R         KEYOFF()

// Note commands
#define O1C       0x0d, 0x5d
#define O1Cs      0x0c, 0x9c
#define O1D       0x0b, 0xe7
#define O1Ds      0x0b, 0x3c
#define O1E       0x0a, 0x9b
#define O1F       0x0a, 0x02
#define O1Fs      0x09, 0x73
#define O1G       0x08, 0xeb
#define O1Gs      0x08, 0x6b
#define O1A       0x07, 0xf2
#define O1As      0x07, 0x80
#define O1B       0x07, 0x14

#define O2C       0x06, 0xaf
#define O2Cs      0x06, 0x4e
#define O2D       0x05, 0xf4
#define O2Ds      0x05, 0x9e
#define O2E       0x05, 0x4e
#define O2F       0x05, 0x01
#define O2Fs      0x04, 0xba
#define O2G       0x04, 0x76
#define O2Gs      0x04, 0x36
#define O2A       0x03, 0xf9
#define O2As      0x03, 0xc0
#define O2B       0x03, 0x8a

#define O3C       0x03, 0x57
#define O3Cs      0x03, 0x27
#define O3D       0x02, 0xfa
#define O3Ds      0x02, 0xcf
#define O3E       0x02, 0xa7
#define O3F       0x02, 0x81
#define O3Fs      0x02, 0x5d
#define O3G       0x02, 0x3b
#define O3Gs      0x02, 0x1b
#define O3A       0x01, 0xfd
#define O3As      0x01, 0xe0
#define O3B       0x01, 0xc5

#define O4C       0x01, 0xac
#define O4Cs      0x01, 0x94
#define O4D       0x01, 0x7d
#define O4Ds      0x01, 0x68
#define O4E       0x01, 0x53
#define O4F       0x01, 0x40
#define O4Fs      0x01, 0x2e
#define O4G       0x01, 0x1d
#define O4Gs      0x01, 0x0d
#define O4A       0x00, 0xfe
#define O4As      0x00, 0xf0
#define O4B       0x00, 0xe3

#define O5C       0x00, 0xd6
#define O5Cs      0x00, 0xca
#define O5D       0x00, 0xbe
#define O5Ds      0x00, 0xb4
#define O5E       0x00, 0xaa
#define O5F       0x00, 0xa0
#define O5Fs      0x00, 0x97
#define O5G       0x00, 0x8f
#define O5Gs      0x00, 0x87
#define O5A       0x00, 0x7f
#define O5As      0x00, 0x78
#define O5B       0x00, 0x71

#define O6C       0x00, 0x6b
#define O6Cs      0x00, 0x65
#define O6D       0x00, 0x5f
#define O6Ds      0x00, 0x5a
#define O6E       0x00, 0x55
#define O6F       0x00, 0x50
#define O6Fs      0x00, 0x4c
#define O6G       0x00, 0x47
#define O6Gs      0x00, 0x43
#define O6A       0x00, 0x40
#define O6As      0x00, 0x3c
#define O6B       0x00, 0x39

#define O7C       0x00, 0x35
#define O7Cs      0x00, 0x32
#define O7D       0x00, 0x30
#define O7Ds      0x00, 0x2d
#define O7E       0x00, 0x2a
#define O7F       0x00, 0x28
#define O7Fs      0x00, 0x26
#define O7G       0x00, 0x24
#define O7Gs      0x00, 0x22
#define O7A       0x00, 0x20
#define O7As      0x00, 0x1e
#define O7B       0x00, 0x1c

#define O8C       0x00, 0x1b
#define O8Cs      0x00, 0x19
#define O8D       0x00, 0x18
#define O8Ds      0x00, 0x16
#define O8E       0x00, 0x15
#define O8F       0x00, 0x14
#define O8Fs      0x00, 0x13
#define O8G       0x00, 0x12
#define O8Gs      0x00, 0x11
#define O8A       0x00, 0x10
#define O8As      0x00, 0x0f
#define O8B       0x00, 0x0e

#endif // SOUND_MACRO_H_
