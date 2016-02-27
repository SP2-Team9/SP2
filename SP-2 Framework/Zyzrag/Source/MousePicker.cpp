#include "MousePicker.h"

/////////////////////////////////////////////////////////////////
/*!

* \file MousePicker.cpp

* \author Goh Zheng Yuen

* \date 16 feb 2016

* This cpp file contains Mouse Picker class which converts the coordinates of the mouse to world coordinates.

*/
/////////////////////////////////////////////////////////////////
MousePicker::MousePicker()
{

}

MousePicker::~MousePicker()
{

}

MousePicker::MousePicker(Camera camera, Mtx44 projection)
{
	this->camera = camera;
	this->projection = projection;
	view.SetToLookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
}

Vector3 MousePicker::getCurrentRay()
{
	return currentRay;
}

void MousePicker::update()
{
	view.SetToLookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	currentRay = calculateMouseRay();
}

Vector3 MousePicker::calculateMouseRay()
{
	double mouseX, mouseY;
	Application::getMouse(mouseX, mouseY);
	x = (2.f * mouseX) / Application::screenWidth - 1.f;
	y = 1.f - (2.f * mouseY) / Application::screenHeight;

	Vector3 clipCoord(x, y, -1.f);
	Vector3 eyeCoords = projection.GetInverse() * clipCoord;

	eyeCoords.Set(eyeCoords.x, eyeCoords.y, -1.f);
	Vector3 ray = view.GetInverse() * eyeCoords;

	return Vector3(ray.x, ray.y, ray.z).Normalized();
}

Vector3 MousePicker::WorldCoord()
{
	Vector3 up(0, 1, 0);
	Vector3 RayOrigin = camera.position;
	Vector3 RayFinal = currentRay * 100000.f;
	Vector3 RayDir = RayFinal - RayOrigin;
	Vector3 PointOnPlane(1, 0, 1);

	// Get D Value
	double d = PointOnPlane.Dot(up);

	// Get T Value
	double t = -(RayOrigin.Dot(up) - d) / RayDir.Dot(up);

	Vector3 ret = RayOrigin + (RayDir * t);
	ret.y = 0;
	return ret;
}

void MousePicker::set(Camera camera, Mtx44 projection)
{
	this->camera = camera;
	this->projection = projection;
}