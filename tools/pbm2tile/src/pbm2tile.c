// -*- coding: utf-8-unix -*-
/**
 * \file pbm2tile.c
 *
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* open_file(const char* file, const char* mode) {
  FILE* f = fopen(file, mode);
  if (!f) {
    fprintf(stderr, "cannot open file -- '%s' (mode: %s)\n", file, mode);
    exit(1);
  }
  return f;
}

void close_file(FILE* f) {
  if (f) {
    fclose(f);
  }
}

// ----
struct parser_state {
  int line;
  int pos;
} parser_state;

void line_pos(void) {
  fprintf(stderr, "at pos = %d, line = %d\n", parser_state.pos, parser_state.line);
}

int get_char(FILE* in) {
  int c = fgetc(in);
  if (c != EOF) {
    parser_state.pos++;
  }
  if (c == '\n') {
    parser_state.line++;
  }
  return c;
}

void unget_char(int c, FILE* in) {
  if (c != EOF) {
    parser_state.pos--;
  }
  if (c == '\n') {
    parser_state.line--;
  }
  c = ungetc(c, in);
  if (c == EOF) {
    perror("ungetc");
    exit(1);
  }
}

_Noreturn
void unexpected_eof(void) {
  line_pos();
  fprintf(stderr, "unexpected end of file\n");
  exit(1);
}

_Noreturn
void unexpected_char(int c, const char* info) {
  line_pos();
  if (c == ' ' || isgraph(c)) {
    fprintf(stderr, "unexpected '%c'\n", c);
  } else {
    fprintf(stderr, "unexpected char (0x%02X)\n", c);
  }
  if (info) {
    fprintf(stderr, "expecting %s\n", info);
  }
  exit(1);
}

void eof(FILE* in) {
  int c = get_char(in);
  if (c != EOF) {
    unexpected_char(c, "end of file");
  }
}

int any_char(FILE* in) {
  int c = get_char(in);
  if (c == EOF) {
    unexpected_eof();
  }
  return c;
}

char char1(FILE* in, int ch) {
  int c = any_char(in);
  if (c != ch) {
    char info[8];
    if (ch == ' ' || isgraph(ch)) {
      snprintf(info, sizeof(info), "'%c'", ch);
    } else {
      snprintf(info, sizeof(info), "(0x%2X)", ch);
    }
    unexpected_char(c, info);
  }
  return c;
}

void space(FILE* in) {
  int c = any_char(in);
  if (isspace(c)) {
    return;
  }
  unexpected_char(c, "white-space");
}

// (line_comment_start, many(any_char), end_of_line)
void line_comment(FILE* in) {
  int c = char1(in, '#');
  do {
    c = get_char(in);
    if (c == EOF) {
      return;
    }
  } while (c != '\n');
}

// space or line_comment
void space_or_line_comment(FILE* in) {
  int c = any_char(in);
  if (isspace(c)) {
    return;
  }
  unget_char(c, in);
  line_comment(in);
}

// some(space or line_comment)
void spaces(FILE* in) {
  space_or_line_comment(in);
  for (;;) {
    int c = get_char(in);
    if (c == EOF) {
      return;
    }
    if (isspace(c)) {
      continue;
    }
    unget_char(c, in);
    if (c != '#') {
      return;
    }
    line_comment(in);
  }
}

int digit(FILE* in) {
  int c = any_char(in);
  if ('0' <= c && c <= '9') {
    return (c - '0');
  }
  unexpected_char(c, "a digit");
}

// some(digit)
int number(FILE* in) {
  int x = digit(in);
  for (;;) {
    int c = get_char(in);
    if (c == EOF) {
      break;
    }
    if ('0' <= c && c <= '9') {
      x = 10 * x + (c - '0');
      continue;
    }
    unget_char(c, in);
    break;
  }
  return x;
}

// ----

_Noreturn
void error_required_argument(char** p) {
  fprintf(stderr, "requires an argument -- '%s'\n", p[0]);
  exit(1);
}

_Noreturn
void error_invalid_argument(char** p) {
  fprintf(stderr, "invalid argument -- '%s %s'\n", p[0], p[1]);
  exit(1);
}

_Noreturn
void error_unknown_option(char** p) {
  fprintf(stderr, "unknown option -- '%s'\n", p[0]);
  exit(1);
}

// ----
void magic_number(FILE* in) {
  char1(in, 'P');
  int x = digit(in);
  if (x < 1 || 6 < x) {
    fprintf(stderr, "illegal PNM format\n");
    exit(1);
  }
  if (x != 4) {
    fprintf(stderr, "unsupported PNM format - magic number 'P%d'\n", x);
    exit(1);
  }
}

// ----

const char * out_file = 0;

void compile(FILE* in, FILE* out) {
  parser_state.line = 1;
  parser_state.pos = 0;
  magic_number(in);
  spaces(in);
  int width = number(in);
  spaces(in);
  int height = number(in);
  space(in);
  if (width <= 0 || height <= 0) {
    fprintf(stderr, "illegal PNM format\n");
    exit(1);
  }
  // ---
  const int cols = (width  + 7) / 8;
  const int rows = (height + 7) / 8;
  const size_t ntiles = cols * rows;
  uint8_t * p = calloc(8, ntiles);
  if (!p) {
    perror("calloc");
    exit(1);
  }
  for (int i = 0; i < rows; ++i) {
    for (int k = 0; k < 8; ++k) {
      uint8_t * q = p + 8 * cols * i + k;
      for (int j = 0; j < cols; ++j) {
        int c = any_char(in);
        *q = ~c & 0xff;
        q += 8;
      }
    }
  }
  eof(in);
  if (fwrite(p, 8, ntiles, out) < ntiles) {
    perror("fwrite");
    exit(1);
  }
  // // debug
  // for (int i = 0; i < ntiles; ++i) {
  //   fprintf(stderr, "(%02X)\n", i);
  //   for (int k = 0; k < 8; ++k) {
  //     int c = p[8 * i + k];
  //     for (int b = 0x80; b > 0; b >>= 1) {
  //       fputc((b & c) ? '1' : '0', stderr);
  //     }
  //     fprintf(stderr, " %02x\n", c);
  //   }
  // }
  free(p);
}

_Noreturn
void help(void) {
  printf("PBM to tile pattern table converter\n");
  printf("Convert the PBM to a series of 1bp tile patterns of 8x8 pixels.\n");
  printf("\n");
  printf("Usage: pbm2tile [OPTION]... FILE...\n");
  printf("\n");
  printf("Options:\n");
  printf("  -h, --help     Display this help and exit\n");
  printf("  -o FILE        output filename\n");
  printf("\n");
  exit(0);
}

static int parse_options(int argc, char** argv) {
  int nargs = 0;
  char** p = argv + 1;
  char** q = argv + 1;
  for (int i = 1; i < argc; ++i) {
    if (!*p) break;
    if (**p != '-') {
      *q++ = *p++;
      nargs++;
      continue;
    }
    if (strcmp(*p, "-h") == 0 || strcmp(*p, "--help") == 0) {
      help();
    }
    if (strcmp(*p, "-o") == 0) {
      if (!p[1]) {
        error_required_argument(p);
      }
      if (p[1][0] == '-' || p[1][0] == '\0') {
        error_invalid_argument(p);
      }
      out_file = p[1];
      p += 2;
      continue;
    }
    error_unknown_option(p);
  }
  return 1 + nargs;
}

int main(int argc, char** argv) {
  FILE* in;
  FILE* out = stdout;
  // display help if no arguments were given
  if (argc <= 1) {
    help();
  }
  // parse command line options
  argc = parse_options(argc, argv);
  //
  if (argc <= 1) {
    fprintf(stderr, "no input files\n");
    fprintf(stderr, "compilation terminated.\n");
    exit(1);
  }
  if (2 < argc) {
    fprintf(stderr, "too much input files\n");
    fprintf(stderr, "compilation terminated.\n");
    exit(1);
  }
  if (out_file) {
    out = open_file(out_file, "w+b");
  }
  in = open_file(argv[1], "rb");
  compile(in, out);
  close_file(in);

  close_file(out);
  return 0;
}
