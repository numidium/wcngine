#include "Texture.h"

Texture loadTexture(SDL_Renderer* renderer, const char* path)
{
	SDL_Surface* surface = SDL_LoadBMP(path);
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));

	Texture texture;
	texture.sdlTexture = SDL_CreateTextureFromSurface(renderer, surface);
	texture.width = surface->w;
	texture.height = surface->h;
	texture.angle = 0;
	SDL_FreeSurface(surface);

	return texture;
}

void unloadTexture(Texture* texture)
{
	SDL_DestroyTexture(texture->sdlTexture);
}

void drawTexture(SDL_Renderer *renderer, Texture* texture, int x, int y, float scaleDivisor, double angle)
{
	SDL_Rect dest;
	dest.w = texture->width / scaleDivisor;
	dest.h = texture->height / scaleDivisor;
	// Origin at center of image
	dest.x = x - dest.w / 2;
	dest.y = y - dest.h / 2;
	if (angle == 0)
	{
		SDL_RenderCopy(renderer, texture->sdlTexture, NULL, &dest);
	}
	else
	{
		SDL_RenderCopyEx(renderer, texture->sdlTexture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
	}
}