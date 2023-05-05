#include "defs.h"

#ifndef STRUCT_HEADERS
#define STRUCT_HEADERS
typedef struct
{
	void (*logic)(void);
	void (*draw)(void);
} Delegate;
typedef struct
{
	SDL_Renderer* renderer;
	SDL_Window* window;
	Delegate delegate;
	int keyboard[MAX_KEYBOARD_KEYS];
} App;
typedef struct
{
	float x;
	float y;
	int width;
	int height;
	float dx;
	float dy;
	int health;
	int reload;
	SDL_Texture* texture;
	Entity* next;
} Entity;

typedef struct Player : Entity
{
	int speed;
};

typedef struct {
	Entity fighterHead, * fighterTail;
	Entity bulletHead, * bulletTail;
};
#endif
