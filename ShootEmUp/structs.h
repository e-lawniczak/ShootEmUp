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
	SDL_Texture* texture;
	Entity* next;
};

typedef struct {
	Entity fighterHead, * fighterTail;
	Entity bulletHead, * bulletTail;
} Stage;
#endif
