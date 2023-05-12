#include "common.h"
#include "sound.h"

static Mix_Chunk* sounds[SND_MAX];
static Mix_Music* music;

void initSounds(void)
{
	memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);

	music = NULL;


	loadSounds();
	loadMusic("sound/theme.mp3");
}

static void loadSounds(void)
{
	sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("sound/player_fire.mp3");
	sounds[SND_ALIEN_FIRE] = Mix_LoadWAV("sound/alien_fire.mp3");
	sounds[SND_PLAYER_DIE] = Mix_LoadWAV("sound/player_die.mp3");
	sounds[SND_ALIEN_DIE] = Mix_LoadWAV("sound/alien_die.mp3");
	sounds[SND_POINTS] = Mix_LoadWAV("sound/point.mp3");
}

void loadMusic(std::string filename)
{
	if (music != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}

	music = Mix_LoadMUS(filename.c_str());
}
void playMusic(int loop)
{
	Mix_PlayMusic(music, (loop) ? -1 : 0);
}
void playSound(int id, int channel)
{
	Mix_PlayChannel(channel, sounds[id], 0);
}