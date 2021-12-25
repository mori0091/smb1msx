// -*- coding: utf-8-unix -*-

#include "smb1.h"

static uBCD16_t remaining;
static uint8_t elapsed;
static bool visible;

void countdown_timer_set_visible(bool b) {
  visible = b;
}

void countdown_timer_print(void) {
  if (!visible) {
    return;
  }
  const uBCD16_t t = remaining;
  set_foreground_color(14);
  locate(212,8);
  text_putc(t < 0x100 ? ' ' : ('0' + ((t >> 8) & 15)));
  text_putc(t < 0x10  ? ' ' : ('0' + ((t >> 4) & 15)));
  text_putc('0' + (t & 15));
}

inline void countdown_timer_print_diff(void) {
  if (!visible) {
    return;
  }
  set_foreground_color(14);

  uBCD16_t t = remaining;
  uint8_t d;

  locate(228, 8);
  d = t & 15;
  text_putc('0' + d);
  if (d < 9) return;
  t >>= 4;

  locate(220, 8);
  if (!t) {
    text_putc(' ');
    return;
  }
  d = t & 15;
  text_putc('0' + d);
  if (d < 9) return;
  t >>= 4;

  locate(212, 8);
  if (!t) {
    text_putc(' ');
    return;
  }
  d = t & 15;
  text_putc('0' + d);
}

void countdown_timer_set(uBCD16_t t) {
  remaining = t;
  elapsed = 0;
}

void countdown_timer_update(void) {
  if (remaining <= 0) {
    event_set(EV_PLAYER_DIES);
    return;
  }
  if (++elapsed == 10) {
    elapsed = 0;
    {
      // remaining--;
      __asm__("ld a, (_remaining)");
      __asm__("dec a");
      __asm__("daa");
      __asm__("ld (_remaining), a");
      __asm__("jr NC, 0001$");
      __asm__("ld a, (_remaining + 1)");
      __asm__("dec a");
      __asm__("ld (_remaining + 1), a");
      __asm__("0001$:");
    }
    countdown_timer_print_diff();
    if (remaining == 0x099) {
      sound_effect(&se_hurryup);
      sound_set_speed((uint8_t)(1.5 * SOUND_SPEED_1X));
    }
  }
}

bool countdown_is_time_up(void) {
  return (remaining <= 0);
}
