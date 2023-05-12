#include "common.h"


void handleKeyUp(SDL_KeyboardEvent* e) {
	//std::cout << "Key up"<<e->keysym.scancode<<"\n" ;
	//std::cout << app.keyboard[e->keysym.scancode]<<"\n" ;

	if (e->repeat == 0 && e->keysym.scancode < MAX_KEYBOARD_KEYS)
	{
		app.keyboard[e->keysym.scancode] = 0;
	}
}
void handleKeyDown(SDL_KeyboardEvent* e) {
	//std::cout << "Key down" << e->keysym.scancode << "\n";
	//std::cout << app.keyboard[e->keysym.scancode] << "\n";

	if (e->repeat == 0 && e->keysym.scancode < MAX_KEYBOARD_KEYS)
	{
		app.keyboard[e->keysym.scancode] = 1;
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


