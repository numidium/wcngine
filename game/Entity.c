#include "Entity.h"

Entity createEntity(EntityPrototype *proto, int x, int y, int z)
{
	Entity entity;
	entity.texture = proto->texture;
	entity.x = x;
	entity.y = y;
	entity.z = z;

	return entity;
}

void drawEntity(Camera *camera, Entity *entity)
{
	double x = entity->x - camera->x;
	double y = entity->y - camera->y;
	double angleToMe = atan2(y, x);
	double angle = camera->angle;
	if (x > 0.0f && y > 0.0f && camera->angle < -M_PI + camera->fov / 2.0f) // Quadrant I
	{
		angle = camera->angle + 2.0f * M_PI;
	}
	else if (x < 0.0f && y > 0.0f && camera->angle < -M_PI + camera->fov / 2.0f) // Quadrant II
	{
		angle = camera->angle + 2.0f * M_PI;
	}
	else if (x < 0.0f && y < 0.0f && camera->angle > M_PI - camera->fov / 2.0f) // Quadrant III
	{
		angle = camera->angle - 2.0f * M_PI;
	}
	else if (x > 0.0f && y < 0.0f && camera->angle > M_PI - camera->fov / 2.0f) // Quadrant IV
	{
		angle = camera->angle - 2.0f * M_PI;
	}

	double camRay = (angle - camera->fov / 2.0f);
	double theta = angleToMe - camRay;
	int screenX = camera->w - (theta / camera->fov) * camera->w;
	int screenY = camera->h / 2;
/*	if (screenX < camera->w + entity->texture.width / 2 
		&& screenX > -entity->texture.width / 2
		&& screenY < camera->h + entity->texture.height / 2
		&& screenY > -entity->texture.height / 2)
	{

	}
*/
	double distance = sqrt(pow(x, 2) + pow(y, 2));
	drawTexture(camera->renderer, &entity->texture, screenX, screenY, distance / 2.0f, 0.0f);
}