#include "Controls.h"

/////////////////////////////////////////////////////////////////
/*!

* \file Controls.cpp

* \author Goh Zheng Yuan

* \date 13 feb 2016

* This contains the class Controls, All movement related are stored here.

*/
/////////////////////////////////////////////////////////////////

bool Controls::cursor = false;

Controls::Controls()
{

}

Controls::~Controls()
{

}

void Controls::FPSMovement(double dt, Camera& camera, vector<AABB> hitbox)
{
	// For looking around
	if (camera.pitch != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(camera.pitch, camera.right.x, camera.right.y, camera.right.z);
		Vector3 check = rotation * camera.view;
		if (check.y < 0.9396 && check.y > -0.9396)
		{
			camera.view = rotation * camera.view;
			camera.target.y = camera.position.y + camera.view.y;
			camera.up = rotation * camera.up;
			camera.right = rotation * camera.right;
		}
	}
	if (camera.yaw != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(camera.yaw, 0, 1, 0);
		camera.view = rotation * camera.view;
		camera.target.x = camera.position.x + camera.view.x;
		camera.target.z = camera.position.z + camera.view.z;
		camera.up = rotation * camera.up;
		camera.right = rotation * camera.right;
	}

	// Walking forward and checking with hitboxes
	if (Application::IsKeyPressed('W'))
	{
		bool checkX, checkZ;
		checkX = checkZ = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (hitbox[i].PointToAABB(Vector3(camera.position.x, camera.position.y, camera.position.z + camera.view.z * dt * camera.cameraSpeed)) && checkZ == false)
			{
				checkZ = true;
			}
			if (hitbox[i].PointToAABB(Vector3(camera.position.x + camera.view.x * dt * camera.cameraSpeed, camera.position.y, camera.position.z)) && checkX == false)
			{
				checkX = true;
			}
		}

		if (checkZ == false)
		{
			camera.target.z += camera.view.z * dt * camera.cameraSpeed;
			camera.position.z += camera.view.z * dt * camera.cameraSpeed;
		}
		if (checkX == false)
		{
			camera.target.x += camera.view.x * dt * camera.cameraSpeed;
			camera.position.x += camera.view.x * dt * camera.cameraSpeed;
		}
	}

	// Walking backward and checking with hitboxes
	if (Application::IsKeyPressed('S'))
	{
		bool checkX, checkZ;
		checkX = checkZ = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (hitbox[i].PointToAABB(Vector3(camera.position.x, camera.position.y, camera.position.z - camera.view.z)) && checkZ == false)
			{
				checkZ = true;
			}
			if (hitbox[i].PointToAABB(Vector3(camera.position.x - camera.view.x, camera.position.y, camera.position.z)) && checkX == false)
			{
				checkX = true;
			}
		}
		if (checkZ == false)
		{
			camera.target.z -= camera.view.z * dt * camera.cameraSpeed;
			camera.position.z -= camera.view.z * dt * camera.cameraSpeed;
		}
		if (checkX == false)
		{
			camera.target.x -= camera.view.x * dt * camera.cameraSpeed;
			camera.position.x -= camera.view.x * dt * camera.cameraSpeed;
		}
	}

	// Walking left and checking with hitboxes
	if (Application::IsKeyPressed('A'))
	{
		bool checkX, checkZ;
		checkX = checkZ = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (hitbox[i].PointToAABB(Vector3(camera.position.x, camera.position.y, camera.position.z - camera.right.z)) && checkZ == false)
			{
				checkZ = true;
			}
			if (hitbox[i].PointToAABB(Vector3(camera.position.x - camera.right.x, camera.position.y, camera.position.z)) && checkX == false)
			{
				checkX = true;
			}
		}

		if (checkZ == false)
		{
			camera.target.z -= camera.right.z * dt * camera.cameraSpeed;
			camera.position.z -= camera.right.z * dt * camera.cameraSpeed;
		}
		if (checkX == false)
		{
			camera.target.x -= camera.right.x * dt * camera.cameraSpeed;
			camera.position.x -= camera.right.x * dt * camera.cameraSpeed;
		}
	}

	// Walking Right and checking with hitboxes
	if (Application::IsKeyPressed('D'))
	{
		bool checkX, checkZ;
		checkX = checkZ = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (hitbox[i].PointToAABB(Vector3(camera.position.x, camera.position.y, camera.position.z + camera.right.z)) && checkZ == false)
			{
				checkZ = true;
			}
			if (hitbox[i].PointToAABB(Vector3(camera.position.x + camera.right.x, camera.position.y, camera.position.z)) && checkX == false)
			{
				checkX = true;
			}
		}

		if (checkZ == false)
		{
			camera.target.z += camera.right.z * dt * camera.cameraSpeed;
			camera.position.z += camera.right.z * dt * camera.cameraSpeed;
		}
		if (checkX == false)
		{
			camera.target.x += camera.right.x * dt * camera.cameraSpeed;
			camera.position.x += camera.right.x * dt * camera.cameraSpeed;
		}
	}

	// Gravity
	bool checkY;
	checkY = false;
	for (int i = 0; i < hitbox.size(); ++i)
	{
		if (hitbox[i].PointToAABB(Vector3(camera.position.x, (camera.position.y - 20.f) - 1.f * dt, camera.position.z)) && checkY == false)
		{
			checkY = true;
		}
	}
	if (checkY == false)
	{
		camera.target.y -= 250.f * dt;
		camera.position.y -= 250.f * dt;
	}
}

void Controls::TPSMovement(double dt, Camera& camera, Vehicles& veh, vector <AABB> hitbox)
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
			camera.target.x += veh.View.x * dt * veh.Thrust;
			camera.position.x += veh.View.x * dt * veh.Thrust;
		}
		if (!checkY)
		{
			veh.Pos.y += veh.View.y * dt * veh.Thrust;
			camera.target.y += veh.View.y * dt * veh.Thrust;
			camera.position.y += veh.View.y * dt * veh.Thrust;
		}
		if (!checkZ)
		{
			veh.Pos.z += veh.View.z * dt * veh.Thrust;
			camera.target.z += veh.View.z * dt * veh.Thrust;
			camera.position.z += veh.View.z * dt * veh.Thrust;
		}

		if (checkX == true && veh.Thrust >= 500 || checkY == true && veh.Thrust >= 500 || checkZ == true && veh.Thrust >= 500)
		{
			veh.isDead = true;
		}

		veh.hitbox = AABB(Vector3(veh.Pos.x - 100, veh.Pos.y - 25, veh.Pos.z - 60), Vector3(veh.Pos.x + 100, veh.Pos.y + 80, veh.Pos.z + 60));
	}

	if (camera.pitch != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(camera.pitch, camera.right.x, camera.right.y, camera.right.z);
		camera.view = rotation * camera.view;
		camera.position = camera.target - camera.view * 200.f;
		camera.up = rotation * camera.up;
		camera.right = rotation * camera.right;
	}
	if (camera.yaw != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(camera.yaw, 0, 1, 0);
		camera.view = rotation * camera.view;
		camera.position.x = camera.target.x - camera.view.x * 200.f;
		camera.position.z = camera.target.z - camera.view.z * 200.f;
		camera.up = rotation * camera.up;
		camera.right = rotation * camera.right;
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
			camera.position = veh.Pos - Vector3(0, 10, -30);
			camera.target = camera.position + Vector3(veh.View.x, 0, veh.View.z);
			camera.view = veh.View;
			camera.right = veh.Right;
			camera.up = camera.right.Cross(camera.view);
			veh.interaction = AABB(Vector3(veh.Pos.x - 20, veh.Pos.y - 30, veh.Pos.z - 20), Vector3(veh.Pos.x + 20, veh.Pos.y, veh.Pos.z + 20));
			veh.board = false;
		}
	}
}

void Controls::NoClip(double dt, Camera& camera)
{
	// For looking around
	if (camera.pitch != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(camera.pitch, camera.right.x, camera.right.y, camera.right.z);
		Vector3 check = rotation * camera.view;
		camera.view = rotation * camera.view;
		camera.target.y = camera.position.y + camera.view.y;
		camera.up = rotation * camera.up;
		camera.right = rotation * camera.right;
	}
	if (camera.yaw != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(camera.yaw, 0, 1, 0);
		camera.view = rotation * camera.view;
		camera.target.x = camera.position.x + camera.view.x;
		camera.target.z = camera.position.z + camera.view.z;
		camera.up = rotation * camera.up;
		camera.right = rotation * camera.right;
	}

	// Walking forward
	if (Application::IsKeyPressed('W'))
	{
		camera.target.y += camera.view.y * dt * camera.cameraSpeed;
		camera.position.y += camera.view.y * dt * camera.cameraSpeed;
		camera.target.x += camera.view.x * dt * camera.cameraSpeed;
		camera.position.x += camera.view.x * dt * camera.cameraSpeed;
		camera.target.z += camera.view.z * dt * camera.cameraSpeed;
		camera.position.z += camera.view.z * dt * camera.cameraSpeed;
	}

	// Walking backward
	if (Application::IsKeyPressed('S'))
	{
		camera.target.x -= camera.view.x * dt * camera.cameraSpeed;
		camera.position.x -= camera.view.x * dt * camera.cameraSpeed;
		camera.target.y -= camera.view.y * dt * camera.cameraSpeed;
		camera.position.y -= camera.view.y * dt * camera.cameraSpeed;
		camera.target.z -= camera.view.z * dt * camera.cameraSpeed;
		camera.position.z -= camera.view.z * dt * camera.cameraSpeed;
	}

	// Walking left
	if (Application::IsKeyPressed('A'))
	{
		camera.target.z -= (float)camera.right.z * dt * camera.cameraSpeed;
		camera.position.z -= (float)camera.right.z * dt * camera.cameraSpeed;
		camera.target.y -= (float)camera.right.y * dt * camera.cameraSpeed;
		camera.position.y -= (float)camera.right.y * dt * camera.cameraSpeed;
		camera.target.x -= (float)camera.right.x * dt * camera.cameraSpeed;
		camera.position.x -= (float)camera.right.x * dt * camera.cameraSpeed;
	}

	// Walking Right
	if (Application::IsKeyPressed('D'))
	{
		camera.target.z += (float)camera.right.z * dt * camera.cameraSpeed;
		camera.position.z += (float)camera.right.z * dt * camera.cameraSpeed;
		camera.target.y += (float)camera.right.y * dt * camera.cameraSpeed;
		camera.position.y += (float)camera.right.y * dt * camera.cameraSpeed;
		camera.target.x += (float)camera.right.x * dt * camera.cameraSpeed;
		camera.position.x += (float)camera.right.x * dt * camera.cameraSpeed;
	}
}

void Controls::YawRotation(double dt, Camera& camera)
{
	camera.yaw;
	Mtx44 rotation;
	rotation.SetToRotation(camera.yaw, 0, 1, 0);
	camera.view = rotation * camera.view;
	camera.position.x = camera.target.x - camera.view.x * 200.f;
	camera.position.z = camera.target.z - camera.view.z * 200.f;
	camera.up = rotation * camera.up;
	camera.right = rotation * camera.right;
}

