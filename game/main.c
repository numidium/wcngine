#include <stdio.h>
#include <SDL.h>
#include <GL/glew.h>
#include <gl/GL.h>
#include <GL/freeglut.h>
#include <SDL_opengl.h>
#include "Texture.h"
#include "Entity.h"
#include "Camera.h"
#pragma comment(lib, "glew32.lib")

void Render();

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
	
	#define ENT_COUNT 2
	Entity entities[ENT_COUNT];
	entities[0] = createEntity(&proto, 10, 0, 0);
	entities[1] = createEntity(&proto, 0, -10, 0);

	// GL rendering settings
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);

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
		/*
		SDL_RenderClear(renderer);
		for (int i = 0; i < ENT_COUNT; i++)
		{
			drawEntity(&camera, &entities[i]);
		}
		SDL_RenderPresent(renderer);
		*/

		SDL_GL_MakeCurrent(window, glContext);
		int width, height;
		SDL_GetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		Render();
		SDL_GL_SwapWindow(window);

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

void Render()
{
	static float color[8][3] = {
	{1.0, 1.0, 0.0},
	{1.0, 0.0, 0.0},
	{0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 1.0},
	{1.0, 1.0, 1.0},
	{1.0, 0.0, 1.0},
	{0.0, 0.0, 1.0}
	};
	static float cube[8][3] = {
		{0.5, 0.5, -0.5},
		{0.5, -0.5, -0.5},
		{-0.5, -0.5, -0.5},
		{-0.5, 0.5, -0.5},
		{-0.5, 0.5, 0.5},
		{0.5, 0.5, 0.5},
		{0.5, -0.5, 0.5},
		{-0.5, -0.5, 0.5}
	};

	/* Do our drawing, too. */
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);

	glColor3fv(color[0]);
	glVertex3fv(cube[0]);
	glColor3fv(color[1]);
	glVertex3fv(cube[1]);
	glColor3fv(color[2]);
	glVertex3fv(cube[2]);
	glColor3fv(color[3]);
	glVertex3fv(cube[3]);

	glColor3fv(color[3]);
	glVertex3fv(cube[3]);
	glColor3fv(color[4]);
	glVertex3fv(cube[4]);
	glColor3fv(color[7]);
	glVertex3fv(cube[7]);
	glColor3fv(color[2]);
	glVertex3fv(cube[2]);

	glColor3fv(color[0]);
	glVertex3fv(cube[0]);
	glColor3fv(color[5]);
	glVertex3fv(cube[5]);
	glColor3fv(color[6]);
	glVertex3fv(cube[6]);
	glColor3fv(color[1]);
	glVertex3fv(cube[1]);

	glColor3fv(color[5]);
	glVertex3fv(cube[5]);
	glColor3fv(color[4]);
	glVertex3fv(cube[4]);
	glColor3fv(color[7]);
	glVertex3fv(cube[7]);
	glColor3fv(color[6]);
	glVertex3fv(cube[6]);

	glColor3fv(color[5]);
	glVertex3fv(cube[5]);
	glColor3fv(color[0]);
	glVertex3fv(cube[0]);
	glColor3fv(color[3]);
	glVertex3fv(cube[3]);
	glColor3fv(color[4]);
	glVertex3fv(cube[4]);

	glColor3fv(color[6]);
	glVertex3fv(cube[6]);
	glColor3fv(color[1]);
	glVertex3fv(cube[1]);
	glColor3fv(color[2]);
	glVertex3fv(cube[2]);
	glColor3fv(color[7]);
	glVertex3fv(cube[7]);

	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glRotatef(5.0, 1.0, 1.0, 1.0);
}