#include "PlayerVehicle.h"

void PlayerVehicle::update(double dt){

    bulletCurrCooldown += dt;
	if (health <= 0)
		isDead = true;
	if (isDead == true)
		respawnTimer += dt;
}

bool PlayerVehicle::respawn(int time)
{
	if (respawnTimer > time)
	{
		respawnTimer = 0;
		isDead = false;
		return true;
	}
	return false;
}

void PlayerVehicle::reset()
{
	thrust = 0;
	yaw = 0;
	pitch = 0;
	health = maxHealth;
}

bool PlayerVehicle::fireBullets(int bulletFireRate){

    float currFireRate = 100 / static_cast<float>(bulletFireRate);

    if (bulletFireRate > 0 && bulletCurrCooldown > currFireRate){
    
         bulletCurrCooldown = 0;

         return true;

     }

    return false;

}
