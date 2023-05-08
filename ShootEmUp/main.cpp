#include "common.h"
#include "init.h"
#include "draw.h"
#include "input.h"
#include "stage.h"
#include "sound.h"
App app;
Entity* player;
Stage stage;


int main(int argc, char* argv[]) {
	long then;
	float remainder;
	srand(time(NULL));

	memset(&app, 0, sizeof(App));
	memset(&player, 0, sizeof(Entity));


	then = SDL_GetTicks();
	remainder = 0;

	initSDL();
	initStage();
	initSounds();
	playMusic(1);
	while (1)
	{
		prepareScene();

		handleInput();

		app.delegate.logic();
		app.delegate.draw();


		presentScene();

		SDL_Delay(16);
	}

	return 0;
}