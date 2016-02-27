#include "Asteroid.h"

Asteroid::Asteroid(float size) : size(size), boom(false)
{
	health = 10 * size;
	speed = 600 / size;
	curRange = 0;
	maxRange = 5000;

    maxHealth = health;
}

Asteroid::~Asteroid()
{

}

void Asteroid::update(double dt)
{
	Pos += View.Normalize() * dt * speed;
	curRange += View.getMagnitude() * dt * speed;

	if (curRange > maxRange)
		boom = true;

	SetHitboxSize(size);
	updateHitbox();
	if (health < 0)
		boom = true;

}