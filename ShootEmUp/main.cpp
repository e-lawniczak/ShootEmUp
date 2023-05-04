#include "common.h"
#include "init.h"
#include "draw.h"
#include "input.h"

App app;
Entity player;

int main(int argc, char* argv[]) {
	memset(&app, 0, sizeof(App));
	memset(&player, 0, sizeof(Entity));

	initSDL();
	player.x = 100;
	player.y = 100;
	player.texture = loadTexture("img/player.png");
	//atexit(cleanup);

	while (1)
	{
		prepareScene();

		handleInput();

		blit(player.texture, player.x, player.y);

		presentScene();

		SDL_Delay(16);
	}
	
	return 0;
}