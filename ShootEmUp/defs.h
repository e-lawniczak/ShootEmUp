#ifndef DEFINE_HEADERS
#define DEFINE_HEADERS
#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720

#define MAX_KEYBOARD_KEYS	350

#define PLAYER_SPEED			4
#define PLAYER_BULLET_SPEED		16
#define PLAYER_RELOAD			10
#define PLAYER_START_HP			3

#define SIDE_PLAYER		0
#define SIDE_ALIEN		1

#define FPS 60

#define ALIEN_BULLET_SPEED    3

#define MAX_STARS	500

#define MAX_SND_CHANNELS 8

#define MAX_LINE_LENGTH 1024
enum
{
	CH_ANY = -1,
	CH_PLAYER,
	CH_ALIEN_FIRE,
	CH_MUSIC,
	CH_ALIEN_DIE,
	CH_POINTS
};

enum
{
	SND_PLAYER_FIRE,
	SND_ALIEN_FIRE,
	SND_PLAYER_DIE,
	SND_ALIEN_DIE,
	SND_MAIN_THEME,
	SND_POINTS,
	SND_MAX
};

#endif