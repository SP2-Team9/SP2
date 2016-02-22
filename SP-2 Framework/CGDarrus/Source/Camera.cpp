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

\param pos - position of camera
\param target - where the camera is looking at
*/
/******************************************************************************/
void Camera::PointAt(Object& obj, float height, float offset)
{
	target = obj.Pos;
	position = obj.Pos - offset;
	position.y = position.y + height;
	view = (target - position);
	right = view.Cross(Vector3(0, 1, 0));
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
			if (hitbox[i].PointToAABB(Vector3(position.x, position.y, position.z - view.z)) && checkZ == false)
			{
				checkZ = true;
			}
			if (hitbox[i].PointToAABB(Vector3(position.x - view.x, position.y, position.z)) && checkX == false)
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
			if (hitbox[i].PointToAABB(Vector3(position.x, position.y, position.z - right.z)) && checkZ == false)
			{
				checkZ = true;
			}
			if (hitbox[i].PointToAABB(Vector3(position.x - right.x, position.y, position.z)) && checkX == false)
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
			if (hitbox[i].PointToAABB(Vector3(position.x, position.y, position.z + right.z)) && checkZ == false)
			{
				checkZ = true;
			}
			if (hitbox[i].PointToAABB(Vector3(position.x + right.x, position.y, position.z)) && checkX == false)
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
		if (hitbox[i].PointToAABB(Vector3(position.x, position.y - 1.f - 250.f * dt, position.z)) && checkY == false)
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

void Camera::TPSMovement(double dt, PlayerVehicle& veh, vector <AABB> hitbox)
{
	getYawAndPitch(dt);

	if (veh.delay > 0)
		veh.delay -= dt;

	if (Application::IsKeyPressed(VK_LSHIFT) && veh.thrust < 1000)
	{
        if (veh.thrust < 30)
		veh.thrust += + 10.f * dt;
	}

	if (Application::IsKeyPressed(VK_LCONTROL))
	{
        if (veh.thrust > -20)
		veh.thrust -= 10.f * dt;
	}

	if (veh.thrust != 0)
	{
		bool checkX, checkY, checkZ;
		checkX = checkY = checkZ = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (veh.hitbox.AABBtoAABB(hitbox[i], Vector3(veh.View.x * dt * veh.thrust, 0, 0)) && checkX == false)
			{
				checkX = true;
			}
			if (veh.hitbox.AABBtoAABB(hitbox[i], Vector3(0, veh.View.y * dt * veh.thrust, 0)) && checkY == false)
			{
				checkY = true;
			}
			if (veh.hitbox.AABBtoAABB(hitbox[i], Vector3(0, 0, veh.View.z * dt * veh.thrust)) && checkZ == false)
			{
				checkZ = true;
			}
		}
		if (!checkX)
		{
			veh.Pos.x += veh.View.x * dt * veh.thrust;
			target.x += veh.View.x * dt * veh.thrust;
			position.x += veh.View.x * dt * veh.thrust;
		}
		if (!checkY)
		{
			veh.Pos.y += veh.View.y * dt * veh.thrust;
			target.y += veh.View.y * dt * veh.thrust;
			position.y += veh.View.y * dt * veh.thrust;
		}
		if (!checkZ)
		{
			veh.Pos.z += veh.View.z * dt * veh.thrust;
			target.z += veh.View.z * dt * veh.thrust;
			position.z += veh.View.z * dt * veh.thrust;
		}

		if (checkX == true && veh.thrust >= 500 || checkY == true && veh.thrust >= 500 || checkZ == true && veh.thrust >= 500)
		{
			veh.isDead = true;
		}

		veh.SetHitboxSize(5);
	}


	if (pitch != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, 0, right.z);
		view = rotation * view;
		position.y = target.y - view.y;
		up = rotation * up;
		right = rotation * right;
	}

	if (yaw != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		position.x = target.x - view.x;
		position.z = target.z - view.z;
		up = rotation * up;
		right = rotation * right;
	}

	if (Application::IsKeyPressed('W'))
	{
		Mtx44 rotation;
		veh.pitch -= dt * 50.f;
		rotation.SetToRotation(-(dt * 50.f), veh.Right.x, veh.Right.y, veh.Right.z);
		veh.View = rotation * veh.View;
		veh.Up = rotation * veh.Up;
		veh.Right = rotation * veh.Right;
	}

	if (Application::IsKeyPressed('S'))
	{
		Mtx44 rotation;
		veh.pitch += dt * 50.f;
		rotation.SetToRotation(dt * 50.f, veh.Right.x, veh.Right.y, veh.Right.z);
		veh.View = rotation * veh.View;
		veh.Up = rotation * veh.Up;
		veh.Right = rotation * veh.Right;
	}

	if (Application::IsKeyPressed('A'))
	{
		Mtx44 rotation;
		veh.yaw += dt * 50.f;
		rotation.SetToRotation(dt * 50.f, 0, 1, 0);
		veh.View = rotation * veh.View;
		veh.Up = rotation * veh.Up;
		veh.Right = rotation * veh.Right;
	}

	if (Application::IsKeyPressed('D'))
	{
		Mtx44 rotation;
		veh.yaw -= dt * 50.f;
		rotation.SetToRotation(-(dt * 50.f), 0, 1, 0);
		veh.View = rotation * veh.View;
		veh.Up = rotation * veh.Up;
		veh.Right = rotation * veh.Right;
	}
}

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

void Camera::YawRotation(double dt)
{
	getYawAndPitch(dt);

	float yaw = 0.f;
	if (Application::IsKeyPressed('A'))
	{
		yaw = -1 * 50.f * dt;
	}
	else if (Application::IsKeyPressed('D'))
	{
		yaw = 50.f * dt;
	}
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	view = rotation * view;
	position = rotation * position;
	up = rotation * up;
	right = rotation * right;
}

void Camera::EnableCursor()
{
	Application::showMouse();
}

void Camera::DisableCursor()
{
	Application::getMouse(mouseX, mouseY);
	Application::centerMouse();
	Application::hideMouse();
}

void Camera::getYawAndPitch(double dt)
{
	yaw = mouseSpeed * dt * static_cast<float>(800 / 2 - mouseX);
	pitch = mouseSpeed * dt * static_cast<float>(600 / 2 - mouseY);
}