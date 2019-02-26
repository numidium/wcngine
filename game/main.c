#include <stdio.h>
#include <SDL.h>
#include <GL/glew.h>
#include <GL/GLU.h>
#include <SDL_opengl.h>
#include "Texture.h"
#include "Entity.h"
#include "Camera.h"
#pragma comment(lib, "glew32.lib")

int main(int argc, char** argv) 
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL failed to initialize: %s\n", SDL_GetError());
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_Window* window = SDL_CreateWindow("WCngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		printf(SDL_GetError());
		return 1;
	}

	// Initialize GLEW
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL)
	{
		printf("Could not create GLContext: %s\n", SDL_GetError());
	}
	GLenum glewInitResult = glewInit();
	if (glewInitResult != GLEW_OK)
	{
		printf("GLEW Initialization failed: %s\n", glewGetErrorString(glewInitResult));
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(renderer, 640, 480);
	
	EntityPrototype proto;
	proto.texture = loadTexture(renderer, "./Assets/Sprites/hyperion.bmp");

	Camera camera = createCamera(renderer);
	
	#define ENT_COUNT 1
	Entity entities[ENT_COUNT];
	entities[0] = createEntity(&proto, 10, 0, 0);

	SDL_Event e;
	while (SDL_WaitEvent(&e))
	{
		short hasQuit = 0;
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
						rotateCameraVert(&camera, -M_PI / 30.0);
						break;
					case SDLK_DOWN:
						rotateCameraVert(&camera, M_PI / 30.0);
						break;
					case SDLK_LEFT:
						rotateCameraHoriz(&camera, M_PI / 30.0);
						break;
					case SDLK_RIGHT:
						rotateCameraHoriz(&camera, -M_PI / 30.0);
						break;
					case SDLK_SPACE:
						moveCamera(&camera, 0.25 * cos(camera.horizAngle), 0.25 * sin(camera.horizAngle), 0.25 * -sin(camera.vertAngle));
						break;
					case SDLK_z:
						rollCamera(&camera, 30.0);
						break;
					case SDLK_x:
						rollCamera(&camera, -30.0);
						break;
					case SDLK_1:
						camera.vertAngle = 0.0;
						break;
					case SDLK_2:
						camera.horizAngle = 0.0;
						break;
					case SDLK_3:
						camera.roll = 0.0;
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