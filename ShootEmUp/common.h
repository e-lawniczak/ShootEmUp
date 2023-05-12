#ifndef COMMON_HEADERS
#define COMMON_HEADERS
#include<SDL.h>
#include<SDL_image.h>
#include <string>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <ctime>
#include <cstdio>

#include "structs.h"
#include "defs.h"
#include "util.h"

extern App app;
extern Stage stage;
extern Entity* player;
extern int highscore;
#endif