#ifndef STAGE_HEADERS
#define STAGE_HEADERS
void initStage();
static void initPlayer();
static void logic();
static void handlePlayer();
static void handleBullets();
static void fireBullet();
static void draw();
static void drawPlayer();
static void drawBullets();
static void capFrameRate(long* then, float* remainder);
static void handleFighters();
static void spawnEnemies();
static void drawFighters();
#endif
