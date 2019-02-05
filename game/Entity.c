#include "Entity.h"
#include "Formulas.h"

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
	// Get origin
	double x = entity->x - camera->x;
	double y = entity->y - camera->y;
	double z = entity->z - camera->z;

	// Handle case where camera is upside-down
	double vertAngle = camera->vertAngle;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (vertAngle > M_PI / 2.0 || vertAngle < -M_PI / 2.0)
	{
		x = -x;
		y = -y;
		flip |= SDL_FLIP_VERTICAL;
		if (vertAngle > 0.0f)
		{
			vertAngle -= M_PI;
		}
		else
		{
			vertAngle += M_PI;
		}
	}

	// Correct the horizontal angle based on Cartesian quadrant
	double horizAngle = camera->horizAngle;
	if (x > 0.0 && y > 0.0 && camera->horizAngle < -M_PI + camera->fov / 2.0) // Quadrant I
	{
		horizAngle = camera->horizAngle + 2.0f * M_PI;
	}
	else if (x < 0.0 && y > 0.0 && camera->horizAngle < -M_PI + camera->fov / 2.0) // Quadrant II
	{
		horizAngle = camera->horizAngle + 2.0f * M_PI;
	}
	else if (x < 0.0 && y < 0.0 && camera->horizAngle > M_PI - camera->fov / 2.0) // Quadrant III
	{
		horizAngle = camera->horizAngle - 2.0f * M_PI;
	}
	else if (x > 0.0 && y < 0.0 && camera->horizAngle > M_PI - camera->fov / 2.0) // Quadrant IV
	{
		horizAngle = camera->horizAngle - 2.0 * M_PI;
	}

	// Get projected X
	double camHorizRay = (horizAngle - camera->fov / 2.0);
	double horizAngleToMe = atan2(y, x);
	double theta = horizAngleToMe - camHorizRay;
	int screenX = camera->w - (theta / camera->fov) * camera->w;

	// Get projected Y
	double distance2d = sqrt(pow(x, 2.0) + pow(y, 2.0));
	double vertAngleToMe = atan2(distance2d, z);
	double camVertRay = (vertAngle - camera->fov / 2.0);
	double iota = vertAngleToMe - camVertRay - M_PI / 2.0;
	int screenY = camera->h - (iota / camera->fov) * camera->h;

	// Adjust for roll
	double rollRadians = degreesToRadians(camera->roll);
	double cartX = screenX - camera->w / 2;
	double cartY = camera->h / 2 - screenY;
	double projTheta = atan2(cartY, cartX);
	projTheta += rollRadians;
	double screenRadius = sqrt(pow(cartX, 2.0) + pow(cartY, 2.0));
	cartX = screenRadius * cos(projTheta);
	cartY = screenRadius * sin(projTheta);
	screenX = cartX + camera->w / 2;
	screenY = camera->h / 2 - cartY;

	// Get texture scale
	double distance = sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
	double distanceDivisor = distance / 2.0;

	// Cull out-of-bounds textures
	if (screenX < camera->w + entity->texture.width / 2 / distanceDivisor
		&& screenX > -entity->texture.width / 2 / distanceDivisor
		&& screenY < camera->h + entity->texture.height / 2 / distanceDivisor
		&& screenY > -entity->texture.height / 2 / distanceDivisor)
	{
		drawTexture(camera->renderer, &entity->texture, screenX, screenY, distanceDivisor, -camera->roll, flip);
	}
}