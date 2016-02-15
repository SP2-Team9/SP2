#ifndef CONTROLS_H
#define CONTROLS_H

#include "Camera.h"
#include "AABB.h"
#include "Application.h"
#include "Object.h"
#include <vector>

using std::vector;

class Controls
{
public:
	Controls();
	~Controls();

	void FPSMovement(double dt, Camera& camera, vector <AABB> hitbox);
	void NoClip(double dt, Camera& camera);
	void TPSMovement(double dt, Camera& camera, Object& obj, vector <AABB> hitbox);
};

#endif