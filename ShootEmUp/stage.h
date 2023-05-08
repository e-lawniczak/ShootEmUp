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
static int bulletHitFighter(Entity* b);
static int fighterHitPlayer(Entity* e, Entity* prev);
static void resetStage();
static void handleEnemies(void);
static void fireAlienBullet(Entity* e);
static void clipPlayer();

#endif
