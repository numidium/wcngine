#include "Camera.h"

Camera createCamera(SDL_Renderer* renderer)
{
	Camera camera;
	camera.renderer = renderer;
	camera.x = 0;
	camera.y = 0;
	camera.z = 0;
	camera.angle = 0.0f;
	camera.fov = M_PI / 2;

	return camera;
}

void unloadCamera(Camera* camera)
{
}