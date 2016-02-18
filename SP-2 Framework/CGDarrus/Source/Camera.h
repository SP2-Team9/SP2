#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Mtx44.h"
#include "Vehicles.h"

class Camera
{
public:
	Vector3 position, target, up, view, right;
	float cameraSpeed, mouseSpeed, pitch, yaw, delay;
	double mouseX, mouseY;

	Camera();
	~Camera();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Init(const Vector3& pos, const Vector3& target);
	void Reset();
	void Update(double dt);

	void PointAt(Object& obj, float height = 0.f, float offset = 0.f);

	void FPSMovement(double dt, vector <AABB> hitbox);
	void NoClip(double dt);
	void TPSMovement(double dt, Vehicles& veh, vector <AABB> hitbox);
	void YawRotation(double dt);
	void EnableCursor();
	void DisableCursor(double dt);
};

#endif