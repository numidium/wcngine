#pragma once
#include <SDL.h>

typedef struct Texture
{
	SDL_Texture* sdlTexture;
	int width, height;
	double angle;
} Texture;

Texture* loadTexture(SDL_Renderer*, const char*);
void drawTexture(SDL_Renderer*, Texture*, int, int, double);