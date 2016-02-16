#ifndef MOUSEPICKER_H
#define MOUSEPICKER_H

#include "Vector3.h"
#include "Mtx44.h"
#include "Vector4.h"
#include "Camera.h"
#include "Application.h"

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
	void update();
	void set(Camera camera, Mtx44 projection);


};

#endif