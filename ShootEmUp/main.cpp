#include "common.h"
#include "init.h"
#include "draw.h"
#include "input.h"

App app;

int main(int argc, char* argv[]) {
	memset(&app, 0, sizeof(App));

	initSDL();

	//atexit(cleanup);

	while (1)
	{
		prepareScene();

		handleInput();

		presentScene();

		SDL_Delay(16);
	}
	
	return 0;
}