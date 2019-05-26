#include "Entity.h"
#include "Formulas.h"

Entity createEntity(EntityPrototype *proto, int x, int y, int z)
{
	Entity entity;
	entity.texture = proto->texture;
	entity.x = (float)x;
	entity.y = (float)y;
	entity.z = (float)z;

	return entity;
}

void drawEntity(Camera *camera, Entity *entity)
{
	
}