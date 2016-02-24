#include "PlayerVehicle.h"



void PlayerVehicle::update(double dt){

    bulletCurrCooldown += dt;


}




bool PlayerVehicle::fireBullets(int bulletFireRate){

    float currFireRate = 100 / static_cast<float>(bulletFireRate);

    if (bulletFireRate > 0 && bulletCurrCooldown > currFireRate){
    
         bulletCurrCooldown = 0;

         return true;

     }

    return false;

}
