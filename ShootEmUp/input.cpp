#include "common.h"


void handleKeyDown(SDL_KeyboardEvent* e) {

	if (e->repeat == 0 && e->keysym.scancode < MAX_KEYBOARD_KEYS)
	{
		app.keyboard[e->keysym.scancode] = 1;
	}
}
void handleKeyUp(SDL_KeyboardEvent* e) {
	if (e->repeat == 0 && e->keysym.scancode < MAX_KEYBOARD_KEYS)
	{
		app.keyboard[e->keysym.scancode] = 0;
	}
}

void handleInput() {
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			exit(0);
			break;
		case SDL_KEYDOWN:
			handleKeyDown(&e.key);
			break;

		case SDL_KEYUP:
			handleKeyUp(&e.key);
			break;
		default:
			break;
		}
	}
}


