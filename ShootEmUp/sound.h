#ifndef SOUND_HEADERS
#define SOUND_HEADERS
void initSounds(void);
static void loadSounds(void);
void loadMusic(std::string filename);
void playMusic(int loop);
void playSound(int id, int channel);
#endif