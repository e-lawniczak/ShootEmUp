#ifndef DRAW_HEADERS
#define DRAW_HEADERS

void prepareScene();
void presentScene();
SDL_Texture* loadTexture(std::string filename);
void blit(SDL_Texture* texture, int x, int y);
#endif
