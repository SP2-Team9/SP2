#include "Camera.h"
#include "Vector3.h"
#include "Application.h"

bool Camera::cursor = false;

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
	mouseSpeed = 7.f;
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
	view = (target - position).Normalized();
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
	if (Application::IsKeyPressed(VK_END) && delay >= 1.f)
	{
		delay = 0;
		if (cursor == true)
			cursor = false;
		else
			cursor = true;
	}

	delay += dt;

	if (cursor == false)
	{
		Application::getMouse(mouseX, mouseY);
		Application::centerMouse();
		Application::hideMouse();
		yaw = mouseSpeed * dt * static_cast<float>(800 / 2 - mouseX);
		pitch = mouseSpeed * dt * static_cast<float>(600 / 2 - mouseY);
	}
	else
	{
		Application::showMouse();
	}


	if (Application::IsKeyPressed(VK_ADD))
		mouseSpeed += 0.1f;
	else if (Application::IsKeyPressed(VK_SUBTRACT))
		mouseSpeed -= 0.1f;

	if (Application::IsKeyPressed(VK_LSHIFT))
		cameraSpeed = 80.f;
	else
		cameraSpeed = 5.f;
}

void Camera::FPSMovement(double dt, vector<AABB> hitbox)
{
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
		if (hitbox[i].PointToAABB(Vector3(position.x, (position.y - 20.f) - 1.f * dt, position.z)) && checkY == false)
		{
			checkY = true;
		}
	}
	if (checkY == false)
	{
		target.y -= 250.f * dt;
		position.y -= 250.f * dt;
	}
}

void Camera::TPSMovement(double dt, Vehicles& veh, vector <AABB> hitbox)
{
	if (veh.delay > 0)
		veh.delay -= dt;
	if (Application::IsKeyPressed(VK_LSHIFT) && veh.Thrust < 1000)
	{
		veh.Thrust += 50.f * dt;
	}

	if (Application::IsKeyPressed(VK_LCONTROL))
	{
		veh.Thrust -= 50.f * dt;
	}

	if (veh.Thrust != 0)
	{
		bool checkX, checkY, checkZ;
		checkX = checkY = checkZ = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (veh.hitbox.AABBtoAABB(hitbox[i], Vector3(veh.View.x * dt * veh.Thrust, 0, 0)) && checkX == false)
			{
				checkX = true;
			}
			if (veh.hitbox.AABBtoAABB(hitbox[i], Vector3(0, veh.View.y * dt * veh.Thrust, 0)) && checkY == false)
			{
				checkY = true;
			}
			if (veh.hitbox.AABBtoAABB(hitbox[i], Vector3(0, 0, veh.View.z * dt * veh.Thrust)) && checkZ == false)
			{
				checkZ = true;
			}
		}
		if (!checkX)
		{
			veh.Pos.x += veh.View.x * dt * veh.Thrust;
			target.x += veh.View.x * dt * veh.Thrust;
			position.x += veh.View.x * dt * veh.Thrust;
		}
		if (!checkY)
		{
			veh.Pos.y += veh.View.y * dt * veh.Thrust;
			target.y += veh.View.y * dt * veh.Thrust;
			position.y += veh.View.y * dt * veh.Thrust;
		}
		if (!checkZ)
		{
			veh.Pos.z += veh.View.z * dt * veh.Thrust;
			target.z += veh.View.z * dt * veh.Thrust;
			position.z += veh.View.z * dt * veh.Thrust;
		}

		if (checkX == true && veh.Thrust >= 500 || checkY == true && veh.Thrust >= 500 || checkZ == true && veh.Thrust >= 500)
		{
			veh.isDead = true;
		}

		veh.hitbox = AABB(Vector3(veh.Pos.x - 100, veh.Pos.y - 25, veh.Pos.z - 60), Vector3(veh.Pos.x + 100, veh.Pos.y + 80, veh.Pos.z + 60));
	}

	if (pitch != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		position = target - view * 200.f;
		up = rotation * up;
		right = rotation * right;
	}
	if (yaw != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation *view;
		position.x = target.x - view.x * 200.f;
		position.z = target.z - view.z * 200.f;
		up = rotation * up;
		right = rotation * right;
	}

	if (Application::IsKeyPressed('W'))
	{
		Mtx44 rotation;
		veh.Pitch -= dt * 50.f;
		rotation.SetToRotation(-(dt * 50.f), veh.Right.x, veh.Right.y, veh.Right.z);
		veh.View = rotation * veh.View;
		veh.Up = rotation * veh.Up;
		veh.Right = rotation * veh.Right;
	}

	if (Application::IsKeyPressed('S'))
	{
		Mtx44 rotation;
		veh.Pitch += dt * 50.f;
		rotation.SetToRotation(dt * 50.f, veh.Right.x, veh.Right.y, veh.Right.z);
		veh.View = rotation * veh.View;
		veh.Up = rotation * veh.Up;
		veh.Right = rotation * veh.Right;
	}

	if (Application::IsKeyPressed('A'))
	{
		Mtx44 rotation;
		veh.Yaw += dt * 50.f;
		rotation.SetToRotation(dt * 50.f, 0, 1, 0);
		veh.View = rotation * veh.View;
		veh.Up = rotation * veh.Up;
		veh.Right = rotation * veh.Right;
	}

	if (Application::IsKeyPressed('D'))
	{
		Mtx44 rotation;
		veh.Yaw -= dt * 50.f;
		rotation.SetToRotation(-(dt * 50.f), 0, 1, 0);
		veh.View = rotation * veh.View;
		veh.Up = rotation * veh.Up;
		veh.Right = rotation * veh.Right;
	}

	if (Application::IsKeyPressed('E') && veh.Thrust < 50.f && veh.delay <= 0)
	{
		bool checkY = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (veh.hitbox.AABBtoAABB(hitbox[i], Vector3(0, -2, 0)) && checkY == false)
			{
				checkY = true;
			}
		}
		if (checkY == true)
		{
			veh.Pitch = 0;
			veh.Thrust = 0;
			veh.View.y = 0;
			position = veh.Pos - Vector3(0, 10, -30);
			target = position + Vector3(veh.View.x, 0, veh.View.z);
			view = veh.View;
			right = veh.Right;
			up = right.Cross(view);
			veh.interaction = AABB(Vector3(veh.Pos.x - 20, veh.Pos.y - 30, veh.Pos.z - 20), Vector3(veh.Pos.x + 20, veh.Pos.y, veh.Pos.z + 20));
			veh.board = false;
		}
	}
}

void Camera::NoClip(double dt)
{
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
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	view = rotation * view;
	position = rotation * position;
	up = rotation * up;
	right = rotation * right;
}