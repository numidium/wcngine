#include "Camera.h"

Camera createCamera(SDL_Renderer* renderer)
{
	Camera camera;
	camera.renderer = renderer;
	camera.x = 0;
	camera.y = 0;
	camera.z = 0;
	camera.angle = 0.0f;
	camera.fov = M_PI / 2.0f;
	SDL_RenderGetLogicalSize(renderer, &camera.w, &camera.h);

	return camera;
}

void rotateCamera(Camera* camera, double delta)
{
	camera->angle += delta;
	if (camera->angle > 2.0f * M_PI - camera->fov / 2.0f)
	{
		camera->angle -= 2.0f * M_PI;
	}
	else if (camera->angle < -2.0f * M_PI + camera->fov / 2.0f)
	{
		camera->angle += 2.0f * M_PI;
	}
}

void moveCamera(Camera* camera, double deltaX, double deltaY)
{
	camera->x += deltaX;
	camera->y += deltaY;
}