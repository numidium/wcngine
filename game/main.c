#include <stdio.h>
#include <SDL.h>
#include "Texture.h"
#include "Entity.h"
#include "Camera.h"

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

	SDL_Window* window = SDL_CreateWindow("WCngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, 0);
	if (window == NULL)
	{
		printf(SDL_GetError());
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(renderer, 640, 480);
	
	EntityPrototype proto;
	proto.texture = loadTexture(renderer, "./Assets/Sprites/hyperion.bmp");

	Camera camera = createCamera(renderer);
	Entity entity = createEntity(&proto, 10, 10, 0);

	SDL_Event e;
	while (SDL_WaitEvent(&e))
	{
		int hasQuit = 0;
		switch (e.type)
		{
			case SDL_QUIT:
				hasQuit = 1;
				printf("User has quit.\n");
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
					case SDLK_LEFT:
						rotateCamera(&camera, M_PI / 18);
						break;
					case SDLK_RIGHT:
						rotateCamera(&camera, -M_PI / 18);
						break;
					case SDLK_1:
						entity.x = 10;
						entity.y = 10;
						break;
					case SDLK_2:
						entity.x = -10;
						entity.y = 10;
						break;
					case SDLK_3:
						entity.x = -10;
						entity.y = -10;
						break;
					case SDLK_4:
						entity.x = 10;
						entity.y = -10;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}

		// main loop
		SDL_RenderClear(renderer);
		drawEntity(&camera, &entity);
		SDL_RenderPresent(renderer);

		if (hasQuit)
		{
			break;
		}
	}

	unloadTexture(&proto.texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}