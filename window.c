#include <SDL2/SDL.h>
#include "window.h"

Window * new_Window(uint32_t width, uint32_t height)
{
  uint32_t i, j;
  Window *w;

  w = malloc(sizeof(Window));
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  w->win = SDL_CreateWindow("VimGL", 
                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                  width * 32, height * 32, 
                  SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
  w->ctx = SDL_GL_CreateContext(w->win);
  if(w->ctx == NULL){
    puts("error: failed to create GL context");
  }
  glewExperimental = GL_TRUE;
  if(glewInit() != GLEW_OK){
    puts("error: failed to initialize GLEW");
  }

  w->w = width;
  w->h = height;

  for(i = 0; i < height; ++i){
    for(j = 0; j < width; ++j){
      w->buff[i][j].ch = rune_blankChar;
    }
  }
  return w;
  GL_CHECK();
}

void del_Window(Window * w)
{
  SDL_GL_DeleteContext(w->ctx);
  SDL_DestroyWindow(w->win);
  free(w);
}

/* window_redraw renders the window by drawing all runes in the render area. */
void window_redraw(Window *w)
{
  unsigned int i, j, k, l;

  /* mark all runes as 'dirty' so that they will be drawn */
  for(i = 0; i < w->h; ++i){
    for(j = 0; j < w->w; ++j){
      w->buff[i][j].r.flags.dirty = true;
    }
  }

  glClear(GL_COLOR_BUFFER_BIT);

  for(i = 0; i < w->h; ++i){
    for(j = 0; j < w->w; ++j){
      /* draw the rune @ (j, i) if it is dirty */
      Rune *r = &w->buff[i][j].r;
      if(r->flags.dirty && r->draw != NULL){
        r->draw(r, j, i);
      }
      /* mark the area that this rune renders to as 'clean' */
      for(k = i; k < (r->h + w->h) && (k < w->h); ++k){
        for(l = j; l < (r->w + w->w) && (l < w->w); ++l){
          r->flags.dirty = false;
        }
      }
    }
  }
  SDL_GL_SwapWindow(w->win);
}

/* window_update updates all runes within the window's render area. */
void window_update(Window *w)
{
  unsigned int i, j, k, l;

  /* mark all runes as 'dirty' so that they will be updated */
  for(i = 0; i < w->h; ++i){
    for(j = 0; j < w->w; ++j){
      w->buff[i][j].r.flags.dirty = true;
    }
  }

  for(i = 0; i < w->h; ++i){
    for(j = 0; j < w->w; ++j){
      /* update the rune @ (j, i) if it is dirty */
      Rune *r;
      r = &(w->buff[i][j].r);
      if(r->flags.dirty && r->update != NULL){
        r->update(r);
      }
      /* mark the area that this rune updated as 'clean' */
      for(k = i; k < (r->h + w->h) && (k < w->h); ++k){
        for(l = j; l < (r->w + w->w) && (l < w->w); ++l){
          r->flags.dirty = false;
        }
      }
    }
  }
}

