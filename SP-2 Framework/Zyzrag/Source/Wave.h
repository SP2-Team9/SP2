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



class Wave{

public:

	Wave();
	~Wave();

	bool waveComplete();
	bool spawnAsteroid();

	void nextWave();
	void waveUpdate(double dt);


	int waveNumber;
	int maxNumberOfAsteroids;
	int numberOfAsteroidsDestroyed;

	double maxWaveCooldownTime;
	double currWaveCooldownTime;

	double maxAsteroidSpawnCooldownTime;
	double currAsteroidSpawnCooldownTime;

};

#endif