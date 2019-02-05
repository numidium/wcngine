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
	
	#define ENT_COUNT 2
	Entity entities[ENT_COUNT];
	entities[0] = createEntity(&proto, 10, 0, 0);
	entities[1] = createEntity(&proto, 0, -10, 0);

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
					case SDLK_UP:
						rotateCameraVert(&camera, -M_PI / 30.0f);
						break;
					case SDLK_DOWN:
						rotateCameraVert(&camera, M_PI / 30.0f);
						break;
					case SDLK_LEFT:
						rotateCameraHoriz(&camera, M_PI / 30.0f);
						break;
					case SDLK_RIGHT:
						rotateCameraHoriz(&camera, -M_PI / 30.0f);
						break;
					case SDLK_SPACE:
						moveCamera(&camera, 0.25f * cos(camera.horizAngle), 0.25f * sin(camera.horizAngle), 0.25f * -sin(camera.vertAngle));
						break;
					case SDLK_z:
						rollCamera(&camera, 5.0f);
						break;
					case SDLK_x:
						rollCamera(&camera, -5.0f);
						break;
					case SDLK_1:
						camera.vertAngle = 0;
						break;
					case SDLK_2:
						camera.horizAngle = 0;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}

		// rendering loop
		SDL_RenderClear(renderer);
		for (int i = 0; i < ENT_COUNT; i++)
		{
			drawEntity(&camera, &entities[i]);
		}
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