/////////////////////////////////////////////////////////////////
/*!

* \file Wave.cpp

* \author: Ashley Wong

* \date:  1 March 2016

* \description: This class defines the functions for Wave class.

*/
/////////////////////////////////////////////////////////////////

#include "Wave.h"

/////////////////////////////////////////////////////////////////
/*!

* \method: constructor

* \author: Wong Keng Han Ashley

* \date: 1 March 2016

* \description: constructor for Wave

*/
/////////////////////////////////////////////////////////////////

Wave::Wave(){

	waveNumber = 1;
	maxNumberOfAsteroids = 5;
	numberOfAsteroidsDestroyed = 0;

	maxWaveCooldownTime = 30;
	currWaveCooldownTime = 30;

	maxAsteroidSpawnCooldownTime = 1;
	currAsteroidSpawnCooldownTime = 0;

    maxTransitionTime = 5;
    currTransitionTime = 0;

    endOfTransition = false;
 
}

/////////////////////////////////////////////////////////////////
/*!

* \method: Destructor

* \author: Wong Keng Han Ashley

* \date: 1 March 2016

* \description: Destructor for Wave

*/
/////////////////////////////////////////////////////////////////

Wave::~Wave(){



}

/////////////////////////////////////////////////////////////////
/*!

* \method: waveComplete

* \author: Wong Keng Han Ashley

* \date: 1 March 2016

* \description: checks if wave is completed
*/
/////////////////////////////////////////////////////////////////

bool Wave::waveComplete(){

	if (numberOfAsteroidsDestroyed >= maxNumberOfAsteroids){
		return true;

	}

	return false;

}

/////////////////////////////////////////////////////////////////
/*!

* \method: loads the next Wave

* \author: Wong Keng Han Ashley

* \date: 1 March 2016

* \description: loads the variables for next wave
*/
/////////////////////////////////////////////////////////////////

void Wave::nextWave(){

	waveNumber += 1;
	maxNumberOfAsteroids += 5;
	numberOfAsteroidsDestroyed = 0;

    currTransitionTime = 0;
	currWaveCooldownTime = 0;

	currAsteroidSpawnCooldownTime = 0;
   

}

/////////////////////////////////////////////////////////////////
/*!

* \method: check if asteroid can spawn

* \author: Wong Keng Han Ashley

* \date: 1 March 2016

* \description: update the wave
*/
/////////////////////////////////////////////////////////////////

void Wave::waveUpdate(double dt){

    currWaveCooldownTime += dt; 

    if (waveComplete()){
 
       
        endOfTransition = false;
        currTransitionTime += dt;

    }

    if (waveComplete() && transitionComplete()){

        endOfTransition = true;
        nextWave();

    }

    if (currWaveCooldownTime > maxWaveCooldownTime){

        currAsteroidSpawnCooldownTime += dt;

    }

}


/////////////////////////////////////////////////////////////////
/*!

* \method: spawn Asteroid

* \author: Wong Keng Han Ashley

* \date: 1 March 2016

* \description: checks if spawning asteroid would be true
*/
/////////////////////////////////////////////////////////////////

bool Wave::spawnAsteroid(){

    if (currWaveCooldownTime > maxWaveCooldownTime){

        if (currAsteroidSpawnCooldownTime > maxAsteroidSpawnCooldownTime && maxNumberOfAsteroids - numberOfAsteroidsDestroyed > 0){

            currAsteroidSpawnCooldownTime = 0;
            return true;

        }

    }

	return false;

}

/////////////////////////////////////////////////////////////////
/*!

* \method: check if transition is complete

* \author: Wong Keng Han Ashley

* \date: 1 March 2016

* \description: check if tansition time is over
*/
/////////////////////////////////////////////////////////////////

bool Wave::transitionComplete(){

    if (currTransitionTime > maxTransitionTime){

        endOfTransition = true;
        return true;

    }
    else{

        return false;

    }

}

