#pragma once
#include <SDL.h>

typedef struct Texture
{
	SDL_Texture *sdlTexture;
	int width, height;
	double angle;
} Texture;

Texture loadTexture(SDL_Renderer *renderer, const char *path);
void unloadTexture(Texture *texture);
void drawTexture(SDL_Renderer *renderer, Texture *texture, int x, int y, float scaleDivisor, double angle);