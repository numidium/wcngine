#pragma once

#include <SDL.h>

typedef struct Camera
{
	SDL_Renderer* renderer;
	double x, y, z, horizAngle, vertAngle, fov, roll;
	int w, h;
} Camera;

Camera createCamera(SDL_Renderer *renderer);
void rotateCameraHoriz(Camera* camera, double delta);
void rotateCameraVert(Camera* camera, double delta);
void moveCamera(Camera* camera, double deltaX, double deltaY, double deltaZ);
void rollCamera(Camera* camera, double deltaRoll);