#include "rune.h"

/* rune_blank represents no character (empty space in buffer) */
CharRune rune_blankChar = {
  .r = {
    .w = 1,
    .h = 1,
    .draw = rune_DrawChar,
    .update = NULL
  },
  .texture = 0
};

MeshRune rune_blankMesh = {
  .r = {
    .w = 1,
    .h = 1,
    .draw = rune_Draw,
    .update = NULL 
  }
};

