#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "window.h"

int main(int argc, char **argv)
{
  bool run;
  SDL_Event evt;
  Window *main_win;

  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    printf("SDL init failed.\n");
    return -1;
  }
  if(TTF_Init() != 0){
    printf("TTF init failed: %s\n", TTF_GetError());
    return -2;
  }

  main_win = new_Window(40, 25);
  window_redraw(main_win);

  /* main loop - handle nvim events */
  for(run=true; run;){
    while(SDL_PollEvent(&evt)){
      switch(evt.type){
        case SDL_QUIT:
         run = false;
         break;
        default:
         break;
      }
    }
    window_update(main_win);
    window_redraw(main_win);
  }
  del_Window(main_win);
  return 0;
}

