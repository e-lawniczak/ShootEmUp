#include "common.h"
#include "draw.h"
#include "stage.h"

static SDL_Texture* bulletTexture;
static SDL_Texture* enemyBulletTexture;
static SDL_Texture* enemyTexture;
static SDL_Texture* harderEnemyTexture;
static SDL_Texture* playerTexture;
static int enemySpawnTimer;
static int stageResetTimer;


void initStage(void) {
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	memset(&stage, 0, sizeof Stage);
	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;

	initPlayer();

	bulletTexture = loadTexture("img/bullet.png");
	enemyBulletTexture = loadTexture("img/enemy_bullet.png");
	enemyTexture = loadTexture("img/enemy.png");
	harderEnemyTexture = loadTexture("img/enemy_2.png");
	playerTexture = loadTexture("img/player.png");

	enemySpawnTimer = 0;

	resetStage();
}
static void resetStage(void)
{
	Entity* e;

	while (stage.fighterHead.next)
	{
		e = stage.fighterHead.next;
		stage.fighterHead.next = e->next;
		free(e);
	}

	while (stage.bulletHead.next)
	{
		e = stage.bulletHead.next;
		stage.bulletHead.next = e->next;
		free(e);
	}

	memset(&stage, 0, sizeof(Stage));
	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;

	initPlayer();

	enemySpawnTimer = 0;

	stageResetTimer = FPS * 2;
}

static void initPlayer(void) {

	player = (Entity*)malloc(sizeof(Entity));
	memset(player, 0, sizeof(Entity));
	stage.fighterTail->next = player;
	stage.fighterTail = player;

	player->x = 100;
	player->y = 100;
	player->h = 76;
	player->w = 76;
	player->health = 1;
	player->side = SIDE_PLAYER;
	player->texture = playerTexture;
	SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

static void logic(void)
{
	handlePlayer();
	handleFighters();
	handleBullets();
	spawnEnemies();
	clipPlayer();
	handleEnemies();

	if (player == NULL && --stageResetTimer <= 0)
	{
		resetStage();
	}
}
static void handlePlayer(void) {
	if (player != NULL)
	{
		player->dx = player->dy = 0;
		player->dx = player->dy = 0;

		if (player->reload > 0)
			player->reload--;

		if (app.keyboard[SDL_SCANCODE_UP])
			player->dy = -PLAYER_SPEED;

		if (app.keyboard[SDL_SCANCODE_DOWN])
			player->dy = PLAYER_SPEED;

		if (app.keyboard[SDL_SCANCODE_LEFT])
			player->dx = -PLAYER_SPEED;

		if (app.keyboard[SDL_SCANCODE_RIGHT])
			player->dx = PLAYER_SPEED;

		if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0)
			fireBullet();

		player->x += player->dx;
		player->y += player->dy;
	}
}
static void handleFighters() {
	Entity* e, * prev;

	prev = &stage.fighterHead;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		e->x += e->dx;
		e->y += e->dy;

		if ((e != player && e->x < -e->w) )
		{
			e->health = 0;
		}
		if ((e != player && fighterHitPlayer(e, prev))) {
			player->texture = NULL;
			player = NULL;
			prev->next = e->next;
			free(e);
			e = prev;
		}

		if (e->health == 0)
		{
			if (e == player)
			{
				player = NULL;
			}

			if (e == stage.fighterTail)
			{
				stage.fighterTail = prev;
			}

			prev->next = e->next;
			free(e);
			e = prev;
		}

		prev = e;
	}
}
static void handleEnemies(void)
{
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		if (e != player && player != NULL && --e->reload <= 0)
		{
			fireAlienBullet(e);
		}
	}
}
static void fireAlienBullet(Entity* e)
{
	Entity* bullet;

	bullet = (Entity*)malloc(sizeof(Entity));
	memset(bullet, 0, sizeof(Entity));
	stage.bulletTail->next = bullet;
	stage.bulletTail = bullet;

	bullet->x = e->x;
	bullet->y = e->y;
	bullet->health = 1;
	bullet->texture = enemyBulletTexture;
	bullet->side = SIDE_ALIEN;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

	bullet->x += (e->w / 2) - (bullet->w / 2);
	bullet->y += (e->h / 2) - (bullet->h / 2);

	calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &bullet->dx, &bullet->dy);

	bullet->dx *= ALIEN_BULLET_SPEED;
	bullet->dy *= ALIEN_BULLET_SPEED;

	e->reload = (rand() % FPS * 2);
}
static int fighterHitPlayer(Entity* e, Entity* prev) {
	//Entity* p = player;
	if (player != NULL && collision(player->x, player->y, player->w, player->h, e->x, e->y, e->w, e->h)) {
		e->health = 0;
		return 1;

	}
	return 0;
}
static void spawnEnemies() {
	Entity* enemy;

	if (--enemySpawnTimer <= 0)
	{
		int isHarder = rand() % 5;
		enemy = (Entity*)malloc(sizeof(Entity));
		memset(enemy, 0, sizeof(Entity));
		stage.fighterTail->next = enemy;
		stage.fighterTail = enemy;

		enemy->x = SCREEN_WIDTH;
		enemy->y = rand() % SCREEN_HEIGHT;
		enemy->side = SIDE_ALIEN;

		if (isHarder == 0) {
			enemy->texture = harderEnemyTexture;
			enemy->health = 4;
			enemy->reload = FPS * (0 + (rand() % 3));
		}
		else {
			enemy->texture = enemyTexture;
			enemy->health = 2;
			enemy->reload = FPS * (2 + (rand() % 3));
		}

		SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

		enemy->dx = -(2 + (rand() % 4));

		enemySpawnTimer = 30 + (rand() % 60);
	}
}
static void handleBullets(void) {
	Entity* b, * prev;

	prev = &stage.bulletHead;

	for (b = stage.bulletHead.next; b != NULL; b = b->next)
	{
		b->x += b->dx;
		b->y += b->dy;

		if (bulletHitFighter(b) || b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT)
		{
			if (b == stage.bulletTail)
			{
				stage.bulletTail = prev;
			}

			prev->next = b->next;
			free(b);
			b = prev;
		}

		prev = b;
	}
}
static int bulletHitFighter(Entity* b)
{
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		if (e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
		{
			b->health = 0;
			e->health = e->health - 1;

			return 1;
		}
	}

	return 0;
}

static void fireBullet(void)
{
	Entity* bullet;

	bullet = (Entity*)malloc(sizeof(Entity));
	memset(bullet, 0, sizeof(Entity));
	stage.bulletTail->next = bullet;
	stage.bulletTail = bullet;

	bullet->x = player->x;
	bullet->y = player->y;
	bullet->w = 10;
	bullet->h = 10;
	bullet->dx = PLAYER_BULLET_SPEED;
	bullet->health = 1;
	bullet->texture = bulletTexture;
	bullet->side = SIDE_PLAYER;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

	bullet->y += (player->h / 2) - (bullet->h / 2);

	player->reload = PLAYER_RELOAD;
}


static void draw(void)
{
	drawPlayer();
	drawBullets();
	drawFighters();
}

static void drawPlayer(void)
{
	if (player != NULL)
		blit(player->texture, player->x, player->y);
}

static void drawBullets(void)
{
	Entity* b;

	for (b = stage.bulletHead.next; b != NULL; b = b->next)
	{
		blit(b->texture, b->x, b->y);
	}
}
static void drawFighters(void)
{
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		blit(e->texture, e->x, e->y);
	}
}

static void capFrameRate(long* then, float* remainder)
{
	long wait, frameTime;
	wait = 16 + *remainder;
	*remainder -= (int)*remainder;
	frameTime = SDL_GetTicks() - *then;
	wait -= frameTime;

	if (wait < 1)
	{
		wait = 1;
	}

	SDL_Delay(wait);
	*remainder += 0.667;
	*then = SDL_GetTicks();
}

static void clipPlayer()
{
	if (player != NULL)
	{
		if (player->x < 0)
		{
			player->x = 0;
		}

		if (player->y < 0)
		{
			player->y = 0;
		}

		if (player->x > SCREEN_WIDTH / 2)
		{
			player->x = SCREEN_WIDTH / 2;
		}

		if (player->y > SCREEN_HEIGHT - player->h)
		{
			player->y = SCREEN_HEIGHT - player->h;
		}
	}
}
