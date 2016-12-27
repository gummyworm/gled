#include <SDL2/SDL.h>
#include <stdbool.h>
#include "gled.h"

int main() {
	bool run;
	SDL_Event evt;

	gled_init();

	for (run = true; run;) {
		/* get input */
		while (SDL_PollEvent(&evt)) {
			switch (evt.type) {
				case SDL_QUIT:
					run = false;
					break;
				default:
					break;
			}
		}

		/* handle nvim events */
	}
	gled_quit();
	return 0;
}
