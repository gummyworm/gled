#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include <stdbool.h>

typedef uint16_t Face[3];
typedef float Color[4];
typedef float Position[3];
typedef float Normal[3];
typedef float Texco[4];

enum {
  RED   = 0,
  GREEN = 1,
  BLUE  = 2,
  ALPHA = 3,

  X = 0,
  Y = 1,
  Z = 2,

  U = 0,
  V = 1
};

typedef struct {
  bool invert;
  bool bold;
  bool italicize;
  bool underline;

  bool dirty;
}RenderFlags;

typedef struct {
  uint32_t font_size;
  uint32_t color;
}RenderProperties;

#endif

