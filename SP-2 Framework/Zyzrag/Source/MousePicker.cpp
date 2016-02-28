/////////////////////////////////////////////////////////////////
/*!

* \file MousePicker.cpp

* \author Goh Zheng Yuen

* \date 16 feb 2016

* This cpp file contains Mouse Picker class which converts the coordinates of the mouse to world coordinates.

*/
/////////////////////////////////////////////////////////////////

#include "MousePicker.h"

/******************************************************************************/
/*!
\brief
	Default Constructor
*/
/******************************************************************************/
MousePicker::MousePicker()
{

}
/******************************************************************************/
/*!
\brief
	Default Destructor
*/
/******************************************************************************/
MousePicker::~MousePicker()
{

}

/******************************************************************************/
/*!
\brief
	Constructor

\param camera
	Gets current camera to pass in to the class.

\param projection
	Gets the projection matrix to  pass in to the class.
*/
/******************************************************************************/
MousePicker::MousePicker(Camera camera, Mtx44 projection)
{
	this->camera = camera;
	this->projection = projection;
	view.SetToLookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
}

/******************************************************************************/
/*!
\brief
	Gets the current ray and return it

\return
	Returns current ray
*/
/******************************************************************************/
Vector3 MousePicker::getCurrentRay()
{
	return currentRay;
}

/******************************************************************************/
/*!
\brief
	Updates the class to the latest camera position, target, up and caluclate the latest mouse ray.
*/
/******************************************************************************/
void MousePicker::update()
{
	view.SetToLookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	currentRay = calculateMouseRay();
}

/******************************************************************************/
/*!
\brief
	Calculate's the mouse ray from screen coordinates to world coordinates.

\return
	Returns a normalized Vector3
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
\brief
	Calculates the coordinates on X and Z world space

\return
	Returns a Vector3 without Y coordinates.
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
\brief
	Sets camera and projection

\param camera
	Current Camera

\param projection
	projection matrix
*/
/******************************************************************************/
void MousePicker::set(Camera camera, Mtx44 projection)
{
	this->camera = camera;
	this->projection = projection;
}