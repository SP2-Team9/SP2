#include "MousePicker.h"

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
	x = (2.f * mouseX) / 800 - 1.f;
	y = 1.0f - (2.0f * mouseY) / 600;

	Vector3 clipCoord(x, y, -1.f);
	Vector3 eyeCoords = projection.GetInverse() * clipCoord;

	eyeCoords.Set(eyeCoords.x, eyeCoords.y, -1.f);
	Vector3 worldCoord = view.GetInverse() * eyeCoords;

	return Vector3(worldCoord.x, worldCoord.y, worldCoord.z);
}

void MousePicker::set(Camera camera, Mtx44 projection)
{
	this->camera = camera;
	this->projection = projection;
}