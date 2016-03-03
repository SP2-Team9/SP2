/////////////////////////////////////////////////////////////////
/*!

* \file Wave.h

* \author: Ashley Wong

* \date:  1 March 2016

* \description: This class contains the Wave class.

*/
/////////////////////////////////////////////////////////////////

#ifndef _WAVE_H
#define _WAVE_H

#include <iostream>

using std::cout;
using std::endl;

class Wave{

public:

	Wave();
	~Wave();

	bool waveComplete();
	bool spawnAsteroid();
    bool transitionComplete();

	void nextWave();
	void waveUpdate(double dt);


    bool endOfTransition;

	int waveNumber;
	int maxNumberOfAsteroids;
	int numberOfAsteroidsDestroyed;

	double maxWaveCooldownTime;
	double currWaveCooldownTime;

    double maxTransitionTime;
    double currTransitionTime;

	double maxAsteroidSpawnCooldownTime;
	double currAsteroidSpawnCooldownTime;

};

#endif