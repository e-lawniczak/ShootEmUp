#include "common.h"
#include "text.h"
#include "draw.h"
#include "stage.h"
#include "sound.h"

static SDL_Texture* bulletTexture;
static SDL_Texture* enemyBulletTexture;
static SDL_Texture* enemyTexture;
static SDL_Texture* harderEnemyTexture;
static SDL_Texture* playerTexture;
static SDL_Texture* background;
static SDL_Texture* explosionTexture;
static int enemySpawnTimer;
static int stageResetTimer;
static Star stars[MAX_STARS];
static int backgroundX = 0;


void initStage(void) {
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	memset(&stage, 0, sizeof Stage);
	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;
	stage.explosionTail = &stage.explosionHead;
	stage.debrisTail = &stage.debrisHead;

	initPlayer();

	bulletTexture = loadTexture("img/bullet.png");
	enemyBulletTexture = loadTexture("img/enemy_bullet.png");
	enemyTexture = loadTexture("img/enemy.png");
	harderEnemyTexture = loadTexture("img/enemy2.png");
	playerTexture = loadTexture("img/player.png");
	background = loadTexture("img/background.png");
	explosionTexture = loadTexture("img/explosion.png");

	enemySpawnTimer = 0;

	resetStage();
}
static void resetStage(void)
{
	Entity* e;
	Explosion* ex;
	Debris* d;

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
	while (stage.explosionHead.next)
	{
		ex = stage.explosionHead.next;
		stage.explosionHead.next = ex->next;
		free(ex);
	}

	while (stage.debrisHead.next)
	{
		d = stage.debrisHead.next;
		stage.debrisHead.next = d->next;
		free(d);
	}

	memset(&stage, 0, sizeof(Stage));
	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;
	stage.explosionTail = &stage.explosionHead;
	stage.debrisTail = &stage.debrisHead;
	stage.score = 0;

	initPlayer();
	initStarfield();

	enemySpawnTimer = 0;

	stageResetTimer = FPS * 3;
	//player->health = PLAYER_START_HP;
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
	player->health = PLAYER_START_HP;
	player->side = SIDE_PLAYER;
	player->texture = playerTexture;
	SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}
static void initStarfield(void)
{
	int i;

	for (i = 0; i < MAX_STARS; i++)
	{
		stars[i].x = rand() % SCREEN_WIDTH;
		stars[i].y = rand() % SCREEN_HEIGHT;
		stars[i].speed = 1 + rand() % 8;
	}
}

static void logic(void)
{
	handleBackground();
	handleStarfield();
	handlePlayer();
	handleFighters();
	handleBullets();
	spawnEnemies();
	clipPlayer();
	handleEnemies();
	handleExplosions();
	handleDebris();

	if (player == NULL && --stageResetTimer <= 0)
	{
		resetStage();
	}
}
static void handleBackground(void)
{
	backgroundX -= 4;
	if (backgroundX < -SCREEN_WIDTH)
	{
		backgroundX = 0;
	}
}
static void handleStarfield(void)
{
	int i;

	for (i = 0; i < MAX_STARS; i++)
	{
		stars[i].x -= stars[i].speed;

		if (stars[i].x < 0)
		{
			stars[i].x = SCREEN_WIDTH + stars[i].x;
		}
	}
}
static void handleExplosions()
{
	Explosion* e, * prev;

	prev = &stage.explosionHead;

	for (e = stage.explosionHead.next; e != NULL; e = e->next)
	{
		e->x += e->dx;
		e->y += e->dy;

		if (--e->a <= 0)
		{
			if (e == stage.explosionTail)
			{
				stage.explosionTail = prev;
			}

			prev->next = e->next;
			free(e);
			e = prev;
		}

		prev = e;
	}
}
static void handleDebris()
{
	Debris* d, * prev;

	prev = &stage.debrisHead;

	for (d = stage.debrisHead.next; d != NULL; d = d->next)
	{
		d->x += d->dx;
		d->y += d->dy;

		d->dy += 0.5;

		if (--d->life <= 0)
		{
			if (d == stage.debrisTail)
			{
				stage.debrisTail = prev;
			}

			prev->next = d->next;
			free(d);
			d = prev;
		}

		prev = d;
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

		if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0) {
			fireBullet();
			playSound(SND_PLAYER_FIRE, CH_PLAYER);
		}

		player->x += player->dx;
		player->y += player->dy;
	}
}
static void addExplosions(int x, int y, int num)
{
	Explosion* e;
	int i;

	for (i = 0; i < num; i++)
	{
		e = (Explosion*)malloc(sizeof(Explosion));
		memset(e, 0, sizeof(Explosion));
		stage.explosionTail->next = e;
		stage.explosionTail = e;

		e->x = x + (rand() % 32) - (rand() % 32);
		e->y = y + (rand() % 32) - (rand() % 32);
		e->dx = (rand() % 10) - (rand() % 10);
		e->dy = (rand() % 10) - (rand() % 10);

		e->dx /= 10;
		e->dy /= 10;


		switch (rand() % 4)
		{
		case 0:
			e->r = 255;
			break;

		case 1:
			e->r = 255;
			e->g = 128;
			break;

		case 2:
			e->r = 255;
			e->g = 255;
			break;

		default:
			e->r = 255;
			e->g = 255;
			e->b = 255;
			break;
		}

		e->a = (rand() % FPS * 3) + 50;
	}
}
static void addDebris(Entity* e)
{
	Debris* d;
	int x, y, w, h;

	w = e->w / 2;
	h = e->h / 2;

	for (y = 0; y <= h; y += h)
	{
		for (x = 0; x <= w; x += w)
		{
			d = (Debris*)malloc(sizeof(Debris));
			memset(d, 0, sizeof(Debris));
			stage.debrisTail->next = d;
			stage.debrisTail = d;

			d->x = e->x + e->w / 2;
			d->y = e->y + e->h / 2;
			d->dx = (rand() % 5) - (rand() % 5);
			d->dy = -(5 + (rand() % 12));
			d->life = FPS * 2;
			d->texture = e->texture;

			d->rect.x = x;
			d->rect.y = y;
			d->rect.w = w;
			d->rect.h = h;
		}
	}
}
static void handleFighters() {
	Entity* e, * prev;

	prev = &stage.fighterHead;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		e->x += e->dx;
		e->y += e->dy;

		if ((e != player && e->x < -e->w))
		{
			e->health = 0;

		}
		if ((e != player && fighterHitPlayer(e, prev))) {
			player->health = player->health - 1;
		}

		if (e->health == 0)
		{

			if (e == player)
			{
				addDebris(player);
				player = NULL;
				playSound(SND_PLAYER_DIE, CH_PLAYER);

			}

			if (e == stage.fighterTail)
			{
				stage.fighterTail = prev;
			}
			if (e != player && e->killed == 1) {
				playSound(SND_ALIEN_DIE, CH_ALIEN_DIE);
				addExplosions(e->x, e->y, 2 + rand() % 4);
				stage.score++;
				highscore = std::max(stage.score, highscore);
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
	playSound(SND_ALIEN_FIRE, CH_ALIEN_FIRE);

}
static int fighterHitPlayer(Entity* e, Entity* prev) {
	//Entity* p = player;
	if (player != NULL && collision(player->x, player->y, player->w, player->h, e->x, e->y, e->w, e->h)) {
		e->health = 0;
		playSound(SND_PLAYER_DIE, CH_PLAYER);
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
		enemy->y = rand() % (SCREEN_HEIGHT - 100);
		enemy->side = SIDE_ALIEN;
		enemy->points = 1;
		enemy->killed = 0;

		if (isHarder == 0) {
			enemy->texture = harderEnemyTexture;
			enemy->health = 4;
			enemy->reload = FPS * (1 + (rand() % 3));
			enemy->w = 60;
			enemy->h = 60;
			enemy->points = 3;
		}
		else {
			enemy->w = 90;
			enemy->h = 90;
			enemy->texture = enemyTexture;
			enemy->health = 2;
			enemy->reload = FPS * (2 + (rand() % 3));
		}

		SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

		enemy->dx = -(2 + (rand() % 4));

		enemySpawnTimer = 15 + (rand() % 25);
	}
}
static void handleBullets(void) {
	Entity* b, * prev;

	prev = &stage.bulletHead;

	for (b = stage.bulletHead.next; b != NULL; b = b->next)
	{
		b->x += b->dx;
		b->y += b->dy;

		if (bulletHitFighter(b) || bulletHitBullet(b) || b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT || b->health == 0)
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

		if (e->side != b->side && b->health > 0 && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
		{

			b->health = 0;
			e->health = e->health - 1;
			if (e->health == 0)
				e->killed = 1;

			return 1;
		}
	}

	return 0;
}
static int bulletHitBullet(Entity* b)
{
	Entity* e;

	for (e = stage.bulletHead.next; e != NULL; e = e->next)
	{
		if (e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
		{
			b->health = 0;
			e->health = 0;

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
	drawBackground();
	drawStarfield();
	drawPlayer();
	drawBullets();
	drawFighters();
	drawExplosions();
	drawDebris();
	drawHud();
}

static void drawHud(void)
{
	drawFont(700, 10, 255, 255, 255, "SCORE: %03d", stage.score);
	if (player != NULL) {
		drawFont(10, 10, 255, 255, 255, "HP: %01d", player->health);
	}
	else {
		drawFont(10, 10, 255, 255, 255, "HP: %01d", 0);
	}

	if (stage.score > 0 && stage.score == highscore)
	{
		drawFont(960, 10, 0, 255, 0, "HIGH SCORE: %03d", highscore);
	}
	else
	{
		drawFont(960, 10, 255, 255, 255, "HIGH SCORE: %03d", highscore);
	}
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
static void drawBackground(void)
{
	SDL_Rect dest;
	int x;

	for (x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
	{
		dest.x = x;
		dest.y = 0;
		dest.w = SCREEN_WIDTH;
		dest.h = SCREEN_HEIGHT;

		SDL_RenderCopy(app.renderer, background, NULL, &dest);
	}
}
static void drawStarfield(void)
{
	int i, c;

	for (i = 0; i < MAX_STARS; i++)
	{
		c = 32 * stars[i].speed;

		SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

		SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
	}
}
static void drawDebris(void)
{
	Debris* d;

	for (d = stage.debrisHead.next; d != NULL; d = d->next)
	{
		blitRect(d->texture, &d->rect, d->x, d->y);
	}
}
static void drawExplosions(void)
{
	Explosion* e;

	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

	for (e = stage.explosionHead.next; e != NULL; e = e->next)
	{

		SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
		SDL_SetTextureAlphaMod(explosionTexture, e->a);

		blit(explosionTexture, e->x, e->y);
	}

	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
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
