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
static int bulletHitBullet(Entity* b);
static int fighterHitPlayer(Entity* e, Entity* prev);
static void resetStage();
static void handleEnemies(void);
static void fireAlienBullet(Entity* e);
static void clipPlayer();
static void initStarfield(void);
static void handleBackground(void);
static void handleStarfield(void);
static void handleExplosions();
static void handleDebris();
static void drawBackground(void);
static void drawStarfield(void);
static void drawDebris(void);
static void drawExplosions(void);
static void drawHud(void);
#endif
