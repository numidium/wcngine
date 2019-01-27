#pragma once

#include <SDL.h>

typedef struct Camera
{
	SDL_Renderer* renderer;
	double x, y, z, angle, fov;
	int w, h;
} Camera;

Camera createCamera(SDL_Renderer *renderer);
void rotateCamera(Camera* camera, double delta);
void moveCamera(Camera* camera, double deltaX, double deltaY);