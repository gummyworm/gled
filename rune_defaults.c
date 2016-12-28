#include "rune.h"

/* rune_blank represents no character (empty space in buffer) */
CharRune rune_blankChar = {
    .r = {.w = 1, .h = 1, .draw = rune_DrawChar, .update = NULL}, .texture = 0};

MeshRune rune_blankMesh = {
    .r = {.w = 1, .h = 1, .draw = rune_DrawMesh, .update = NULL},
    .x = 0,
    .y = 0,
    .mesh = {.vertices = NULL,
	     .numVertices = 0,
	     .faces = NULL,
	     .numFaces = 0,
	     .vao = 0,
	     .ibo = 0,
	     .vbo = 0,
	     .fbo = 0,
	     .color = 0,
	     .depth = 0}};

ImgRune rune_blankImg = {
    .r = {.w = 8, .h = 8, .draw = rune_DrawImg, .update = NULL},
    .texture = 0,
    .filename = NULL};
