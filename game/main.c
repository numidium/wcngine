#include <stdio.h>
#include <SDL.h>
#include "Texture.h"

int main(int argc, char** argv) 
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf(SDL_GetError());
	}
	else
	{
		printf("SDL appears to be working.\n");
	}

	SDL_Window* window = SDL_CreateWindow("WCngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	if (window == NULL)
	{
		printf(SDL_GetError());
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	Texture* texture = loadTexture(renderer, "./Assets/Sprites/hyperion.bmp");
	
	SDL_RenderClear(renderer);
	drawTexture(renderer, texture, 0, 0, 45);
	SDL_RenderPresent(renderer);

	SDL_Event e;
	while (SDL_WaitEvent(&e))
	{
		switch (e.type)
		{
			case SDL_QUIT:
				printf("User has quit.\n");
				break;
		}
	}

	SDL_DestroyTexture(texture->sdlTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}