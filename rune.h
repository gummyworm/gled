/*
 * rune.h
 * Runes represent one cell in the screen's buffer. They may be characters,
 * images, models, etc.
 * Runes are drawn with the rune_Draw() function.
 * Runes may be animated in the rune_Update() function.
 */
#ifndef RUNE_H
#define RUNE_H

#include <SDL2/SDL.h>
#include "mesh.h"
#include "render.h"
#include "vector.h"

/* Dimensional limitations of each rune */
enum{
  RUNE_MAX_W = 80,
  RUNE_MAX_H = 80
};

/* The codepages that each rune maps to */
enum{
  CODEPAGE_CHAR = 0x00, /* 0x00-0xe000: UTF-8 characters */
  CODEPAGE_RSV  = 0xe000, /* 0xe000-0xe008: reserved */
  CODEPAGE_RSRC = 0xe008, /* 0xe008-0xf8ff: resources */
  CODEPAGE_END  = 0xf8ff /* end of supported range */
};

/* RuneDrawTarget contains the information used to apply a rune's render */
typedef struct{
  GLuint tex;
  Rect clip;
}RuneDrawResult;

/* Rune_ is the basic struct (and first member) of all rune types */
typedef struct Rune{
  RenderFlags      flags;
  RenderProperties props;

  uint32_t type;
  uint32_t w, h;  /* the dimensions (in cells) that this rune renders to */

  RuneDrawResult (*draw)(struct Rune *r, uint32_t x, uint32_t y);
  void (*update)(struct Rune*);
}Rune;

/* CharRune is a rune representing 1 1x1 cell symbol */
typedef struct{
  Rune r;
  uint32_t f, v;    /* f (fragment), v (vertex) shader handles */
  uint32_t id;      /* id is a texture ID */
  uint32_t texture; /* texture handle for the character's spritesheet */
}CharRune;

/* MeshRune represents 1 cell of an arbitrary dimension 3D-mesh */
typedef struct{
  Rune r;
  uint32_t x, y;    /* (x, y) is the coords relative to the upper-left corner */
  uint32_t f, v;    /* f (fragment), v (vertex) shader handles */
  Mesh mesh;        /* mesh to render */
}MeshRune;

/* Rune is a container large enough to hold any Rune type */
typedef union {
  Rune r;
  CharRune ch;
  MeshRune mesh;
}Rune_;

Rune *new_CharRune();
void del_Rune(Rune*);

void rune_Draw(Rune*, uint32_t, uint32_t);
RuneDrawResult rune_DrawChar(Rune*, uint32_t, uint32_t);

void rune_Update(Rune*);

extern CharRune rune_blankChar;
extern MeshRune rune_blankMesh;


#endif

