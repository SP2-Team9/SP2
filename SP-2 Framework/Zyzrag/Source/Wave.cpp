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
	currWaveCooldownTime = 0;

	maxAsteroidSpawnCooldownTime = 1;
	currAsteroidSpawnCooldownTime = 0;
 
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

	if (numberOfAsteroidsDestroyed == maxNumberOfAsteroids){

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

	if (waveComplete()){

		currWaveCooldownTime += dt;

	}

	if (currWaveCooldownTime > maxWaveCooldownTime){

		nextWave();

	}

	currAsteroidSpawnCooldownTime += dt;

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

	if (currAsteroidSpawnCooldownTime > maxAsteroidSpawnCooldownTime){

		currAsteroidSpawnCooldownTime = 0;
		return true;

	}

	return false;

}
