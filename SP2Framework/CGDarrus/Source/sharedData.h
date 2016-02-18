#ifndef _SHAREDDATA_H
#define _SHAREDDATA_H

class sharedData{

public:


	~sharedData();


	sharedData getInputCooldown(){

		static sharedData inputCooldown;

		return inputCooldown;

	}



private:

	sharedData();
	double inputCooldown;
	double cooldownDuration;



};



#endif _SHAREDDATA_H