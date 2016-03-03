/////////////////////////////////////////////////////////////////
/*!

* \file Camera.cpp

* \author: Goh Zheng Yuan

* \date: 15 feb 2016

* \description: This cpp contains the functions of the class Camera

*/
/////////////////////////////////////////////////////////////////

#include "Camera.h"
#include "Vector3.h"
#include "Application.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera::Camera()
{
	lock = false;
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
Initialize camera

\param pos - position of camera
\param target - where the camera is looking at
*/
/******************************************************************************/
void Camera::Init(const Vector3& pos, const Vector3& target)
{
	this->position = pos;
	this->target = target;
	view = (target - position).Normalized();
	right = view.Cross(Vector3(0, 1, 0));
	up = right.Cross(view);

	cameraSpeed = 20.f;
	mouseSpeed = 8.f;
}

/******************************************************************************/
/*!
\brief
Points at object

\param obj - The object that the camera wants to look at.
\param height - Height away from the object
\param offset - Z distance away from object.
*/
/******************************************************************************/
void Camera::PointAt(Object& obj, float height, float offset)
{
	target = obj.Pos;
	position = target + (obj.View * offset);
	position += obj.Up * height;
	view = position - target;
	right = view.Cross(obj.Up);
	up = right.Cross(view);
}

/******************************************************************************/
/*!
\brief
Reset the camera settings
*/
/******************************************************************************/
void Camera::Reset(){




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
	if (Application::IsKeyPressed(VK_ADD))
		mouseSpeed += 0.1f;
	else if (Application::IsKeyPressed(VK_SUBTRACT))
		mouseSpeed -= 0.1f;

	if (Application::IsKeyPressed(VK_LSHIFT))
		cameraSpeed = 10.f;
	else
		cameraSpeed = 5.f;
}

/******************************************************************************/
/*!
\brief
First Person camera movement

\param dt - Delta time.
\param hitbox - Vector of AABB hitbox to test if camera is colliding with anything.
*/
/******************************************************************************/
void Camera::FPSMovement(double dt, vector<AABB> hitbox)
{
	getYawAndPitch(dt);

	// For looking around
	if (pitch != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		Vector3 check = rotation * view;
		if (check.y < 0.9396 && check.y > -0.9396)
		{
			view = rotation * view;
			target.y = position.y + view.y;
			up = rotation * up;
			right = rotation * right;
		}
	}
	if (yaw != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target.x = position.x + view.x;
		target.z = position.z + view.z;
		up = rotation * up;
		right = rotation * right;
	}

	// Walking forward and checking with hitboxes
	if (Application::IsKeyPressed('W'))
	{
		bool checkX, checkZ;
		checkX = checkZ = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (hitbox[i].PointToAABB(Vector3(position.x, position.y, position.z + view.z * dt * cameraSpeed)) && checkZ == false)
			{
				checkZ = true;
			}
			if (hitbox[i].PointToAABB(Vector3(position.x + view.x * dt * cameraSpeed, position.y, position.z)) && checkX == false)
			{
				checkX = true;
			}
		}

		if (checkZ == false)
		{
			target.z += view.z * dt * cameraSpeed;
			position.z += view.z * dt * cameraSpeed;
		}
		if (checkX == false)
		{
			target.x += view.x * dt * cameraSpeed;
			position.x += view.x * dt * cameraSpeed;
		}
	}

	// Walking backward and checking with hitboxes
	if (Application::IsKeyPressed('S'))
	{
		bool checkX, checkZ;
		checkX = checkZ = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (hitbox[i].PointToAABB(Vector3(position.x, position.y, position.z - view.z * dt * cameraSpeed)) && checkZ == false)
			{
				checkZ = true;
			}
			if (hitbox[i].PointToAABB(Vector3(position.x - view.x * dt * cameraSpeed, position.y, position.z)) && checkX == false)
			{
				checkX = true;
			}
		}
		if (checkZ == false)
		{
			target.z -= view.z * dt * cameraSpeed;
			position.z -= view.z * dt * cameraSpeed;
		}
		if (checkX == false)
		{
			target.x -= view.x * dt * cameraSpeed;
			position.x -= view.x * dt * cameraSpeed;
		}
	}

	// Walking left and checking with hitboxes
	if (Application::IsKeyPressed('A'))
	{
		bool checkX, checkZ;
		checkX = checkZ = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (hitbox[i].PointToAABB(Vector3(position.x, position.y, position.z - right.z * dt * cameraSpeed)) && checkZ == false)
			{
				checkZ = true;
			}
			if (hitbox[i].PointToAABB(Vector3(position.x - right.x * dt * cameraSpeed, position.y, position.z)) && checkX == false)
			{
				checkX = true;
			}
		}

		if (checkZ == false)
		{
			target.z -= right.z * dt * cameraSpeed;
			position.z -= right.z * dt * cameraSpeed;
		}
		if (checkX == false)
		{
			target.x -= right.x * dt * cameraSpeed;
			position.x -= right.x * dt * cameraSpeed;
		}
	}

	// Walking Right and checking with hitboxes
	if (Application::IsKeyPressed('D'))
	{
		bool checkX, checkZ;
		checkX = checkZ = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (hitbox[i].PointToAABB(Vector3(position.x, position.y, position.z + right.z * dt * cameraSpeed)) && checkZ == false)
			{
				checkZ = true;
			}
			if (hitbox[i].PointToAABB(Vector3(position.x + right.x * dt * cameraSpeed, position.y, position.z)) && checkX == false)
			{
				checkX = true;
			}
		}

		if (checkZ == false)
		{
			target.z += right.z * dt * cameraSpeed;
			position.z += right.z * dt * cameraSpeed;
		}
		if (checkX == false)
		{
			target.x += right.x * dt * cameraSpeed;
			position.x += right.x * dt * cameraSpeed;
		}
	}

	// Gravity
	bool checkY;
	checkY = false;
	for (int i = 0; i < hitbox.size(); ++i)
	{
		if (hitbox[i].PointToAABB(Vector3(position.x, position.y - 1.f, position.z)) == true && checkY == false)
		{
			checkY = true;
			break;
		}
	}
	if (checkY == false)
	{
		target.y -= 250.f * dt;
		position.y -= 250.f * dt;
	}
}

/******************************************************************************/
/*!
\brief
Third person movement

\param dt - Delta time.
\param veh - Player's vehicle to control.
*/
/******************************************************************************/
void Camera::TPSMovement(double dt, PlayerVehicle& veh)
{
	if (veh.delay > 0)
		veh.delay -= dt;

	if (Application::IsKeyPressed(VK_MENU) && sharedData::GetInstance()->Delay(0.5f))
	{
		lock = (lock + 1) % 2;
	}

	if (lock == true)
	{
		getYawAndPitch(dt);
		target = veh.Pos;
		if (pitch != 0)
		{
			Mtx44 rotation;
			rotation.SetToRotation(pitch * -1, right.x, 0, right.z);
			view = rotation * view;
			up = rotation * up;
			right = rotation * right;
			
		}

		if (yaw != 0)
		{
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			up = rotation * up;
			right = rotation * right;
		}
		position = target + view;
	}
	else
	{
		PointAt(veh, 5, -30);
	}

	if (Application::IsKeyPressed(VK_LSHIFT) && veh.thrust < 100)
	{
		veh.thrust += + 10.f * dt;
	}

	if (Application::IsKeyPressed(VK_LCONTROL) && veh.thrust > -40)
	{
		veh.thrust -= 10.f * dt;
	}
	
	if (Application::IsKeyPressed('W'))
	{
		Mtx44 rotation;
		veh.pitch -= dt * 50.f;
		rotation.SetToRotation(-(dt * 50.f), veh.Right.x, 0, veh.Right.z);
		veh.View = rotation * veh.View;
		veh.Up = rotation * veh.Up;
		veh.Right = rotation * veh.Right;
	}

	if (Application::IsKeyPressed('S'))
	{
		Mtx44 rotation;
		veh.pitch += dt * 50.f;
		rotation.SetToRotation(dt * 50.f, veh.Right.x, 0, veh.Right.z);
		veh.View = rotation * veh.View;
		veh.Up = rotation * veh.Up;
		veh.Right = rotation * veh.Right;
	}

	if (Application::IsKeyPressed('A'))
	{
		Mtx44 rotation;
		veh.yaw += dt * 50.f;
		rotation.SetToRotation(dt * 50.f, 0,1,0);
		veh.View = rotation * veh.View;
		veh.Up = rotation * veh.Up;
		veh.Right = rotation * veh.Right;
	}

	if (Application::IsKeyPressed('D'))
	{
		Mtx44 rotation;
		veh.yaw -= dt * 50.f;
		rotation.SetToRotation(-(dt * 50.f), 0,1,0);
		veh.View = rotation * veh.View;
		veh.Up = rotation * veh.Up;
		veh.Right = rotation * veh.Right;
	}
}

/******************************************************************************/
/*!
\brief
	Camera mode that is not affected by any hitboxes

\param dt - Delta Time.
*/
/******************************************************************************/
void Camera::NoClip(double dt)
{
	getYawAndPitch(dt);

	// For looking around
	if (pitch != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		Vector3 check = rotation * view;
		view = rotation * view;
		target.y = position.y + view.y;
		up = rotation * up;
		right = rotation * right;
	}
	if (yaw != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target.x = position.x + view.x;
		target.z = position.z + view.z;
		up = rotation * up;
		right = rotation * right;
	}

	// Walking forward
	if (Application::IsKeyPressed('W'))
	{
		target.y += view.y * dt * cameraSpeed;
		position.y += view.y * dt * cameraSpeed;
		target.x += view.x * dt * cameraSpeed;
		position.x += view.x * dt * cameraSpeed;
		target.z += view.z * dt * cameraSpeed;
		position.z += view.z * dt * cameraSpeed;
	}

	// Walking backward
	if (Application::IsKeyPressed('S'))
	{
		target.x -= view.x * dt * cameraSpeed;
		position.x -= view.x * dt * cameraSpeed;
		target.y -= view.y * dt * cameraSpeed;
		position.y -= view.y * dt * cameraSpeed;
		target.z -= view.z * dt * cameraSpeed;
		position.z -= view.z * dt * cameraSpeed;
	}

	// Walking left
	if (Application::IsKeyPressed('A'))
	{
		target.z -= right.z * dt * cameraSpeed;
		position.z -= right.z * dt * cameraSpeed;
		target.y -= right.y * dt * cameraSpeed;
		position.y -= right.y * dt * cameraSpeed;
		target.x -= right.x * dt * cameraSpeed;
		position.x -= right.x * dt * cameraSpeed;
	}

	// Walking Right
	if (Application::IsKeyPressed('D'))
	{
		target.z += right.z * dt * cameraSpeed;
		position.z += right.z * dt * cameraSpeed;
		target.y += right.y * dt * cameraSpeed;
		position.y += right.y * dt * cameraSpeed;
		target.x += right.x * dt * cameraSpeed;
		position.x += right.x * dt * cameraSpeed;
	}
}

/******************************************************************************/
/*!
\brief
	Camera mode that only allows Yaw Rotation and controlling vehicle's thrust.

\param veh - Player's vehicle to control.
\param dt - Delta time.
*/
/******************************************************************************/
void Camera::YawRotation(PlayerVehicle& veh, double dt)
{
	getYawAndPitch(dt);

	float yaw = 0.f;
	if (Application::IsKeyPressed('A'))
	{
		yaw = -1 * 80.f * dt;
	}
	else if (Application::IsKeyPressed('D'))
	{
		yaw = 80.f * dt;
	}

	if (Application::scrollY > 0 && view.getMagnitude() > 50)
	{
		Vector3 normView = view.Normalized();
		view -= normView * 10;
		Application::scrollY = 0;
	}

	if (Application::scrollY < 0 && view.getMagnitude() < 300)
	{
		Vector3 normView = view.Normalized();
		view += normView * 10;
		Application::scrollY = 0;
	}

	target = veh.Pos;
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	view = rotation * view;
	position = target + view;
	up = rotation * up;
	right = rotation * right;
}

/******************************************************************************/
/*!
\brief
	Enable mouse cursor.
*/
/******************************************************************************/
void Camera::EnableCursor()
{
	Application::showMouse();
}

/******************************************************************************/
/*!
\brief
	Disable mouse cursor.
*/
/******************************************************************************/
void Camera::DisableCursor()
{
	Application::getMouse(mouseX, mouseY);
	Application::centerMouse();
	Application::hideMouse();
}

/******************************************************************************/
/*!
\brief
	Sets Yaw and Pitch from mouse movement

\param dt - Delta time.
*/
/******************************************************************************/
void Camera::getYawAndPitch(double dt)
{
	yaw = mouseSpeed * dt * static_cast<float>(Application::screenWidth/ 2 - mouseX);
	pitch = mouseSpeed * dt * static_cast<float>(Application::screenHeight / 2 - mouseY);
}