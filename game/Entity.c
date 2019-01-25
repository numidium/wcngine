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
	double toOther = atan2(y, x);
	double camRay = (camera->angle - camera->fov / 2.0f);
	// camera angle points to center of fov
	double theta = toOther - camRay;
	int screenX = 640 - (theta / camera->fov) * 640;
	int screenY = 240;
	drawTexture(camera->renderer, &entity->texture, screenX, screenY, 1.0f, 0.0f);
}