#include "common.h"
#include "init.h"
#include "draw.h"
#include "input.h"

App app;
Player player;
Entity bullet;

void handlePlayerMovement() {
	player.x += player.dx;
	player.y += player.dy;
	if (app.up)
		player.y -= player.speed;

	if (app.down)
		player.y += player.speed;

	if (app.left)
		player.x -= player.speed;

	if (app.right)
		player.x += player.speed;
}
void handleFire() {
	printf("%d %d", app.fire, bullet.health);
	if (app.fire && bullet.health == 0) {
		bullet.x = player.x;
		bullet.y = player.y + 25;
		bullet.dx = 16;
		bullet.dy = 0;
		bullet.health = 1;
	}
	bullet.x += bullet.dx;
	bullet.y += bullet.dy;

	if (bullet.x > SCREEN_WIDTH)
	{
		bullet.health = 0;
	}

	blit(player.texture, player.x, player.y);

	if (bullet.health > 0)
	{
		blit(bullet.texture, bullet.x, bullet.y);
	}
}
int main(int argc, char* argv[]) {
	memset(&app, 0, sizeof(App));
	memset(&player, 0, sizeof(Entity));
	memset(&bullet, 0, sizeof(Entity));

	initSDL();
	player.x = 100;
	player.y = 100;
	player.texture = loadTexture("img/player.png");
	player.speed = 4;
	bullet.texture = loadTexture("img/bullet.png");

	while (1)
	{
		prepareScene();

		handleInput();
		handleFire();

		handlePlayerMovement();
		blit(player.texture, player.x, player.y);

		presentScene();

		SDL_Delay(16);
	}
	
	return 0;
}