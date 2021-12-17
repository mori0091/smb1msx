// -*- coding: utf-8-unix -*-

#include "smb1.h"

static int remaining;
static uint8_t elapsed;

// void countdown_timer_print(void) {
//   int t = remaining;
//   set_foreground_color(14);
//   locate(212,8);
//   if (t < 10) {
//     text_puts("  ");
//   }
//   else {
//     if (t < 100) {
//       text_puts(" ");
//     }
//     else {
//       text_putc('0' + t / 100);
//       t %= 100;
//     }
//     text_putc('0' + t / 10);
//     t %= 10;
//   }
//   text_putc('0' + t);
// }
void countdown_timer_print(void) {
  int t = remaining;
  set_foreground_color(14);
  locate(212,8);
  text_putc(t < 0x100 ? ' ' : ('0' + ((t >> 8) & 15)));
  text_putc(t < 0x10  ? ' ' : ('0' + ((t >> 4) & 15)));
  text_putc('0' + (t & 15));
}

// void countdown_timer_set(int t) {
//   remaining = t;
//   elapsed = 0;
// }
void countdown_timer_set(int t) {
  remaining = ((t % 1000) / 100 * 256) + ((t % 100) / 10 * 16) + (t % 10);
  elapsed = 0;
}

// void countdown_timer_update(void) {
//   if (remaining <= 0) {
//     event_set(EV_PLAYER_DIES);
//     return;
//   }
//   if (++elapsed == 10) {
//     elapsed = 0;
//     remaining--;
//     countdown_timer_print();
//   }
// }
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
    countdown_timer_print();
    if (remaining == 0x099) {
      sound_effect(&se_hurryup);
    }
    if (remaining == 0x091) {
      sound_set_speed((uint8_t)(1.5 * SOUND_SPEED_1X));
    }
  }
}

bool countdown_is_time_up(void) {
  return (remaining <= 0);
}