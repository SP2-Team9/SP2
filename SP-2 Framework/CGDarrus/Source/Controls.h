#ifndef CONTROLS_H
#define CONTROLS_H

#include "Camera.h"
#include "Application.h"
#include "Vehicles.h"
#include <vector>

using std::vector;


/////////////////////////////////////////////////////////////////
/*!

* \file Controls.h

* \author Goh Zheng Yuan

* \date 13 feb 2016

* This contains the class Controls, All movement related are stored here.

*/
/////////////////////////////////////////////////////////////////
class Controls
{
private:
	double mouseX, mouseY;
	float  mouseSpeed, pitch, yaw;
	static bool cursor;

public:
	Controls();
	~Controls();

	void FPSMovement(double dt, Camera& camera, vector <AABB> hitbox);
	void NoClip(double dt, Camera& camera);
	void TPSMovement(double dt, Camera& camera, Vehicles& veh, vector <AABB> hitbox);
	void YawRotation(double dt, Camera& camera);
};

#endif