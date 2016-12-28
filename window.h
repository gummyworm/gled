#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include "rune.h"

enum { WINDOW_MAX_W = 480, WINDOW_MAX_H = 300 };

typedef struct {
	uint32_t w, h;
	SDL_Window *win;
	SDL_GLContext ctx;

	Rune_ buff[WINDOW_MAX_W][WINDOW_MAX_H];

	const char name[32];
} Window;

Window *new_Window(uint32_t, uint32_t);
void del_Window(Window *);

void window_redraw(Window *);
void window_update(Window *);
void window_resize(Window *, uint32_t, uint32_t);
Rune_ *window_at(Window *, uint32_t, uint32_t);

void window_setChar(Window *, uint32_t, uint32_t, CharRune *);
void window_setMesh(Window *, uint32_t, uint32_t, MeshRune *);
void window_setImg(Window *, uint32_t, uint32_t, ImgRune *);

#endif
