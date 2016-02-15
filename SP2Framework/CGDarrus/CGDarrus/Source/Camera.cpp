#include "Camera.h"
#include "Vector3.h"
#include "Application.h"

//double Camera::mouseX = 0;
//double Camera::mouseY = 0;

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera::Camera()
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera::~Camera()
{
}

/******************************************************************************/
/*!
\brief
Initialize camera

\param pos - position of camera
\param target - where the camera is looking at
\param up - up vector of camera
*/
/******************************************************************************/
void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
	view = (target - position).Normalized();
	right = view.Cross(up);
	right.y = 0;
	right.Normalized();

	cameraSpeed = 20.f;
	mouseSpeed = 7.f;
}

/******************************************************************************/
/*!
\brief
Reset the camera settings
*/
/******************************************************************************/
void Camera::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void Camera::Update(double dt)
{
	Application::getMouse(mouseX, mouseY);
	yaw = mouseSpeed * dt * static_cast<float>(800 / 2 - mouseX);
	pitch = mouseSpeed * dt * static_cast<float>(600 / 2 - mouseY);

	if (Application::IsKeyPressed(VK_ADD))
		mouseSpeed += 0.1f;
	else if (Application::IsKeyPressed(VK_SUBTRACT))
		mouseSpeed -= 0.1f;

	if (Application::IsKeyPressed(VK_LSHIFT))
		cameraSpeed = 80.f;
	else
		cameraSpeed = 50.f;
}