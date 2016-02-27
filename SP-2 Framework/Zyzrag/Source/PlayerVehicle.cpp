#include "PlayerVehicle.h"

void PlayerVehicle::update(double dt, vector<AABB> hitbox){

    bulletCurrCooldown += dt;
	if (health <= 0)
	{
		thrust = 0;
		Pos.SetZero();
		isDead = true;
	}
		
	if (isDead == true)
		respawnTimer += dt;

	if (thrust != 0)
	{
		bool checkX, checkY, checkZ;
		checkX = checkY = checkZ = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (this->hitbox.AABBtoAABB(hitbox[i], Vector3(View.x * dt * thrust, 0, 0)) && checkX == false)
			{
				checkX = true;
			}
			if (this->hitbox.AABBtoAABB(hitbox[i], Vector3(0, View.y * dt * thrust, 0)) && checkY == false)
			{
				checkY = true;
			}
			if (this->hitbox.AABBtoAABB(hitbox[i], Vector3(0, 0, View.z * dt * thrust)) && checkZ == false)
			{
				checkZ = true;
			}
		}
		if (!checkX)
		{
			Pos.x += View.x * dt * thrust;
		}
		if (!checkY)
		{
			Pos.y += View.y * dt * thrust;
		}
		if (!checkZ)
		{
			Pos.z += View.z * dt * thrust;
		}

		if (checkX || checkY || checkZ)
		{
			if (thrust > 30)
			{
				health -= thrust;
				thrust *= -1;
				if (thrust < -40)
					thrust = -40;
			}
			
		}
		updateHitbox();
	}
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
