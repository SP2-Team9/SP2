#ifndef _SHAREDDATA_H
#define _SHAREDDATA_H

class sharedData{
public:

	static sharedData* GetInstance()
	{
		static sharedData data;
		return &data;
	}

	bool Delay(double time)
	{
		if (delay > time)
		{
			delay = 0;
			return true;
		}
		return false;
	}

	void DelayIncrement(double dt, double time = 0.5f)
	{
		if (delay < time)
		{
			delay += dt;
		}
	}

	bool quit = false;


private:

	sharedData(){};

	float delay;

};



#endif _SHAREDDATA_H