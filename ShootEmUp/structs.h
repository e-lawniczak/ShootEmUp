#include "defs.h"

#ifndef STRUCT_HEADERS
#define STRUCT_HEADERS
typedef struct
{
	void (*logic)();
	void (*draw)();
} Delegate;
typedef struct
{
	SDL_Renderer* renderer;
	SDL_Window* window;
	Delegate delegate;
	int keyboard[MAX_KEYBOARD_KEYS];
} App;
struct Entity {
	float x;
	float y;
	int w;
	int h;
	float dx;
	float dy;
	int side;
	int health;
	int reload;
	int points;
	int killed;
	SDL_Texture* texture;
	Entity* next;
};
struct Explosion {
	float x;
	float y;
	float dx;
	float dy;
	int r, g, b, a;
	Explosion* next;
};

struct Debris {
	float x;
	float y;
	float dx;
	float dy;
	SDL_Rect rect;
	SDL_Texture* texture;
	int life;
	Debris* next;
};
typedef struct {
	Entity fighterHead, * fighterTail;
	Entity bulletHead, * bulletTail;
	Explosion explosionHead, * explosionTail;
	Debris debrisHead, * debrisTail;
	Entity pointsHead, * pointsTail;
	int score;
} Stage;


typedef struct {
	int x;
	int y;
	int speed;
} Star;
#endif
