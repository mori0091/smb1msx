// -*- coding: utf-8-unix -*-

#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

_Noreturn
void unexpected_eof(void) {
  fprintf(stderr, "unexpected end of file\n");
  exit(1);
}

_Noreturn
void unexpected_char(int c, const char* info) {
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

void skip_spaces(FILE* in) {
  for (;;) {
    int c = fgetc(in);
    if (c == EOF) {
      return;
    }
    if (isspace(c)) {
      continue;
    }
    if (c == '#') {
      do {
        c = fgetc(in);
        if (c == EOF) return;
      } while (c != '\n');
      continue;
    }
    c = ungetc(c, in);
    if (c == EOF) {
      perror("ungetc");
      exit(1);
    }
    return;
  }
}

int hex_digit(FILE* in) {
  int c = fgetc(in);
  if ('0' <= c && c <= '9') {
    return (c - '0');
  }
  if ('a' <= c && c <= 'f') {
    return (c - 'a' + 10);
  }
  if ('A' <= c && c <= 'F') {
    return (c - 'A' + 10);
  }
  /* hex character not found */
  unexpected_char(c, "a hex digit");
}

int pat_digit(FILE* in) {
  int c = fgetc(in);
  if ('0' <= c && c <= '3') {
    return (c - '0');
  }
  if (c == '.') {
    return 3;
  }
  /* pattern character ('0', '1', '2', '3', or '.') not found */
  unexpected_char(c, "a pattern digit");
}

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

_Noreturn
void help(void) {
  printf("Tileset Compiler\n");
  printf("Creates '4-color tilesets' from input FILE(s).\n");
  printf("\n");
  printf("Usage: tilec [OPTION]... FILE...\n");
  printf("\n");
  printf("Options:\n");
  printf("  -h, --help     Display this help and exit\n");
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
    error_unknown_option(p);
  }
  return 1 + nargs;
}

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

void write_header(FILE* f) {
  fprintf(f,
          "// -*- coding: utf-8-unix -*-\n"
          "\n"
          "#include <stdint.h>\n"
          "#include <stdlib.h>\n"
          "#include \"tileset.h\"\n"
          "\n"
          );
}

void compile(FILE* in, FILE* out_index, FILE* out_patterns) {
  for (;;) {
    skip_spaces(in);
    if (feof(in)) break;
    uint8_t id = (uint8_t)(hex_digit(in) << 4 | hex_digit(in));
    skip_spaces(in);
    uint16_t upper_colors = (((uint16_t)hex_digit(in) << 12) |
                             ((uint16_t)hex_digit(in) <<  8) |
                             ((uint16_t)hex_digit(in) <<  4) |
                             ((uint16_t)hex_digit(in)));
    skip_spaces(in);
    uint16_t lower_colors = (((uint16_t)hex_digit(in) << 12) |
                             ((uint16_t)hex_digit(in) <<  8) |
                             ((uint16_t)hex_digit(in) <<  4) |
                             ((uint16_t)hex_digit(in)));
    skip_spaces(in);

    fprintf(out_index,
            "  0x%02x, TILE_COLOR(%04X, %04X),\n",
            (int)id, (int)upper_colors, (int)lower_colors);

    fprintf(out_patterns,
            "  // 0x%02x, TILE_COLOR(%04X, %04X),\n",
            (int)id, (int)upper_colors, (int)lower_colors);
    for (int i = 0; i < 16; ++i) {
      uint64_t hex = 0;
      uint32_t x = 0;
      for (int j = 0; j < 16; ++j) {
        int d = pat_digit(in);
        x <<= 2;
        x |= d;
        hex <<= 4;
        hex |= d;
      }
      fprintf(out_patterns,
              "  0x%02x, 0x%02x, 0x%02x, 0x%02x,  // %016" PRIX64 "\n",
              (x >> 24) & 255,
              (x >> 16) & 255,
              (x >>  8) & 255,
              (x >>  0) & 255,
              hex);
      skip_spaces(in);
    }
  }
}

int main(int argc, char** argv) {
  FILE* in = stdin;
  FILE* out_index = 0;
  FILE* out_patterns = 0;
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
  out_index = open_file("tileset_table.c", "w+");
  out_patterns = open_file("tileset_patterns.c", "w+");
  // -- header
  write_header(out_index);
  fprintf(out_index, "const uint8_t tileset_table[] = {\n");
  write_header(out_patterns);
  fprintf(out_patterns, "const uint8_t tile_patterns[] = {\n");

  for (int i = 1; i < argc; ++i) {
    in = open_file(argv[i], "r");
    compile(in, out_index, out_patterns);
    close_file(in);
  }
  fprintf(out_index,
          "};\n"
          "\n"
          "const size_t tileset_table_size = sizeof(tileset_table);\n");
  fprintf(out_patterns, "};\n");
  close_file(out_index);
  close_file(out_patterns);
  return 0;
}
