#pragma once
#include <math.h>
#include "Texture.h"
#include "Camera.h"

typedef struct Entity
{
	float x, y, z;
	Texture texture;
} Entity;

typedef struct EntityPrototype
{
	Texture texture;
} EntityPrototype;

Entity createEntity(EntityPrototype *proto, int x, int y, int z);
void drawEntity(Camera *camera, Entity *entity);