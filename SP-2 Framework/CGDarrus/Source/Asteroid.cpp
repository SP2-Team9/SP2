#include "Asteroid.h"

Asteroid::Asteroid(float size) : size(size), boom(false)
{
	health = 10 * size;
	speed = 500 / size;
}

Asteroid::~Asteroid()
{

}

void Asteroid::update(double dt)
{
	Pos += View.Normalize() * dt * speed;
	SetHitboxSize(size);
	if (health < 0)
		boom = true;
}