#include "Texture.h"

Texture* loadTexture(SDL_Renderer* renderer, const char* path)
{
	SDL_Surface* surface = SDL_LoadBMP(path);
	if (surface == NULL)
	{
		return NULL;
	}
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
	Texture* texture = malloc(sizeof(Texture));
	texture->sdlTexture = SDL_CreateTextureFromSurface(renderer, surface);
	texture->width = surface->w;
	texture->height = surface->h;
	texture->angle = 0;
	SDL_FreeSurface(surface);

	return texture;
}

void drawTexture(SDL_Renderer* renderer, Texture* texture, int x, int y, double angle)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = texture->width;
	dest.h = texture->height;
	if (angle == 0)
	{
		SDL_RenderCopy(renderer, texture->sdlTexture, NULL, &dest);
	}
	else
	{
		SDL_RenderCopyEx(renderer, texture->sdlTexture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
	}
}