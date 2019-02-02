#include "Camera.h"

Camera createCamera(SDL_Renderer* renderer)
{
	Camera camera;
	camera.renderer = renderer;
	camera.x = 0;
	camera.y = 0;
	camera.z = 0;
	camera.horizAngle = 0.0f;
	camera.vertAngle = 0.0f;
	camera.fov = M_PI / 2.0f;
	SDL_RenderGetLogicalSize(renderer, &camera.w, &camera.h);

	return camera;
}

void rotateCameraHoriz(Camera* camera, double delta)
{
	camera->horizAngle += delta;
	if (camera->horizAngle > 2.0f * M_PI - camera->fov / 2.0f)
	{
		camera->horizAngle -= 2.0f * M_PI;
	}
	else if (camera->horizAngle < -2.0f * M_PI + camera->fov / 2.0f)
	{
		camera->horizAngle += 2.0f * M_PI;
	}
}

void rotateCameraVert(Camera* camera, double delta)
{
	camera->vertAngle += delta;
	if (camera->vertAngle > (2.0f * M_PI - camera->fov) / 2.0f)
	{
		camera->vertAngle -= 2.0f * M_PI;
	}
	else if (camera->vertAngle < (-2.0f * M_PI + camera->fov) / 2.0f)
	{
		camera->vertAngle += 2.0f * M_PI;
	}
}

void moveCamera(Camera* camera, double deltaX, double deltaY, double deltaZ)
{
	if (camera->vertAngle > M_PI / 2 || camera->vertAngle < -M_PI / 2)
	{
		camera->x -= deltaX;
		camera->y -= deltaY;
		camera->z -= deltaZ;
	}
	else
	{
		camera->x += deltaX;
		camera->y += deltaY;
		camera->z += deltaZ;
	}
}