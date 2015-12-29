#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gled.h"
#include "window.h"

static Window *main_win;

int gled_init()
{
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
    printf("SDL init failed.\n");
    return -1;
  }
  if(TTF_Init() != 0){
    printf("TTF init failed: %s\n", TTF_GetError());
    return -2;
  }

  main_win = new_Window(40, 25);
  if(main_win == NULL){
    return -3;
  }
  window_redraw(main_win);
  return 0;
}

void gled_quit()
{
  del_Window(main_win);
  SDL_Quit();
}

void gled_redraw()
{
  window_redraw(main_win);
}

void gled_update()
{
  window_update(main_win);
  window_redraw(main_win);
}

void gled_clear()
{
}

void gled_resize(uint64_t cols, uint64_t rows)
{
  window_resize(main_win, cols, rows);
}

void gled_onmousepress(uint64_t x, uint64_t y)
{

}

void gled_onmouserelease(uint64_t x, uint64_t y)
{

}

