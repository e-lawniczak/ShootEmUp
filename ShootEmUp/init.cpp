#include "common.h"


void initSDL() {
	int rendererFlags, windowFlags;
	rendererFlags = SDL_RENDERER_ACCELERATED;
	windowFlags = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Initialize error: %s", SDL_GetError());
		exit(1);
	}

	app.window = SDL_CreateWindow("Shooter 01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

	if (!app.window)
	{
		printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

	if (!app.renderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0) {
		printf("IMG initialize error: %s", SDL_GetError());
		exit(1);
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		printf("Couldn't initialize SDL Mixer\n");
		exit(1);
	}
	int vol1 = 2;
	int vol2 = 30;
	Mix_AllocateChannels(MAX_SND_CHANNELS);
	Mix_Volume(CH_ALIEN_FIRE, vol1);
	Mix_Volume(CH_ANY, vol1);
	Mix_Volume(CH_PLAYER, vol1);
	Mix_Volume(CH_MUSIC, 5);
	Mix_Volume(CH_ALIEN_DIE, vol1);
	Mix_VolumeMusic(vol2);

	SDL_ShowCursor(0);
}