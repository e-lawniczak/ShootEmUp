#include "common.h"
#include "draw.h"

SDL_Texture* fontTexture;
char drawTextBuffer[MAX_LINE_LENGTH];
int GLYPH_WIDTH = 1000;
int GLYPH_HEIGHT = 50;
TTF_Font* arial;
void initFonts() {
	arial = TTF_OpenFont("font/arial.ttf", 18);

	//fontTexture = loadTexture("font/font.png");
}

void drawText(int x, int y, int r, int g, int b, std::string format, ...)
{

	int i, len, c;
	SDL_Rect rect;
	va_list args;

	memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

	va_start(args, format);
	vsprintf_s(drawTextBuffer, format.c_str(), args);
	va_end(args);

	len = strlen(drawTextBuffer);

	rect.w = GLYPH_WIDTH;
	rect.h = GLYPH_HEIGHT;
	rect.y = 0;

	SDL_SetTextureColorMod(fontTexture, r, g, b);

	std::cout << drawTextBuffer << std::endl;

	for (i = 0; i < len; i++)
	{
		c = drawTextBuffer[i];

		if (c >= ' ' && c <= 'Z')
		{
			rect.x = (c - ' ') * GLYPH_WIDTH;

			blitRect(fontTexture, &rect, x, y);

			x += GLYPH_WIDTH;
		}
	}
}

void drawFont(int x, int y, int r, int g, int b, std::string format, ...) {
	SDL_Texture* texture;
	va_list args;

	SDL_Surface* surface;
	SDL_Color textColor = { r, g, b };
	va_start(args, format);
	vsprintf_s(drawTextBuffer, format.c_str(), args);
	va_end(args);

	surface = TTF_RenderText_Solid(arial, drawTextBuffer, textColor);
	texture = SDL_CreateTextureFromSurface(app.renderer, surface);

	
	blit(texture, x, y);
}
