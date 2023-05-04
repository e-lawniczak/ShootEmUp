#include "common.h"

void handleInput() {
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			exit(0);
			break;

		default:
			break;
		}
	}
}