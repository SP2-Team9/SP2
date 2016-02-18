#ifndef MOUSEPICKER_H
#define MOUSEPICKER_H

#include "Vector3.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Application.h"

/////////////////////////////////////////////////////////////////
/*!

* \file MousePicker.cpp

* \author Goh Zheng Yuen

* \date 16 feb 2016

* This cpp file contains Mouse Picker class which converts the coordinates of the mouse to world coordinates.

*/
/////////////////////////////////////////////////////////////////
class MousePicker
{
private:
	Vector3 currentRay;
	Mtx44 projection, view;
	Camera camera;
	float x, y;

public:
	MousePicker();
	MousePicker(Camera camera, Mtx44 projection);
	~MousePicker();
	Vector3 getCurrentRay();
	Vector3 calculateMouseRay();
	Vector3 WorldCoord();
	void update();
	void set(Camera camera, Mtx44 projection);


};

#endif