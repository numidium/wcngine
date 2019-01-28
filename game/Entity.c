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
	double z = entity->z - camera->z;
	double horizAngleToMe = atan2(y, x);
	double horizAngle = camera->horizAngle;
	if (x > 0.0f && y > 0.0f && camera->horizAngle < -M_PI + camera->fov / 2.0f) // Quadrant I
	{
		horizAngle = camera->horizAngle + 2.0f * M_PI;
	}
	else if (x < 0.0f && y > 0.0f && camera->horizAngle < -M_PI + camera->fov / 2.0f) // Quadrant II
	{
		horizAngle = camera->horizAngle + 2.0f * M_PI;
	}
	else if (x < 0.0f && y < 0.0f && camera->horizAngle > M_PI - camera->fov / 2.0f) // Quadrant III
	{
		horizAngle = camera->horizAngle - 2.0f * M_PI;
	}
	else if (x > 0.0f && y < 0.0f && camera->horizAngle > M_PI - camera->fov / 2.0f) // Quadrant IV
	{
		horizAngle = camera->horizAngle - 2.0f * M_PI;
	}

	double camHorizRay = (horizAngle - camera->fov / 2.0f);
	double theta = horizAngleToMe - camHorizRay;
	int screenX = camera->w - (theta / camera->fov) * camera->w;

	double distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	double vertAngleToMe = atan2(distance, z);
	double vertAngle = camera->vertAngle;
	double camVertRay = (vertAngle - camera->fov / 2.0f);
	double iota = vertAngleToMe - camVertRay - M_PI / 2;
	int screenY = camera->h - (iota / camera->fov) * camera->h;

	double distanceDivisor = distance / 2.0f;
	if (screenX < camera->w + entity->texture.width / 2 / distanceDivisor
		&& screenX > -entity->texture.width / 2 / distanceDivisor
		&& screenY < camera->h + entity->texture.height / 2 / distanceDivisor
		&& screenY > -entity->texture.height / 2 / distanceDivisor)
	{
		drawTexture(camera->renderer, &entity->texture, screenX, screenY, distanceDivisor, 0.0f);
	}
}