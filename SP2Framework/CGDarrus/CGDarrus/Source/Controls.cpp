#include "Controls.h"

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

void Controls::TPSMovement(double dt, Camera& camera, Object& obj, vector <AABB> hitbox)
{
	if (obj.delay > 0)
		obj.delay -= dt;
	if (Application::IsKeyPressed(VK_LSHIFT) && obj.Thrust < 1000)
	{
		obj.Thrust += 50.f * dt;
	}

	if (Application::IsKeyPressed(VK_LCONTROL))
	{
		obj.Thrust -= 50.f * dt;
	}

	if (obj.Thrust != 0)
	{
		bool checkX, checkY, checkZ;
		checkX = checkY = checkZ = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (obj.hitbox.AABBtoAABB(hitbox[i], Vector3(obj.View.x * dt * obj.Thrust, 0, 0)) && checkX == false)
			{
				checkX = true;
			}
			if (obj.hitbox.AABBtoAABB(hitbox[i], Vector3(0, obj.View.y * dt * obj.Thrust, 0)) && checkY == false)
			{
				checkY = true;
			}
			if (obj.hitbox.AABBtoAABB(hitbox[i], Vector3(0, 0, obj.View.z * dt * obj.Thrust)) && checkZ == false)
			{
				checkZ = true;
			}
		}
		if (!checkX)
		{
			obj.Pos.x += obj.View.x * dt * obj.Thrust;
			camera.target.x += obj.View.x * dt * obj.Thrust;
			camera.position.x += obj.View.x * dt * obj.Thrust;
		}
		if (!checkY)
		{
			obj.Pos.y += obj.View.y * dt * obj.Thrust;
			camera.target.y += obj.View.y * dt * obj.Thrust;
			camera.position.y += obj.View.y * dt * obj.Thrust;
		}
		if (!checkZ)
		{
			obj.Pos.z += obj.View.z * dt * obj.Thrust;
			camera.target.z += obj.View.z * dt * obj.Thrust;
			camera.position.z += obj.View.z * dt * obj.Thrust;
		}

		if (checkX == true && obj.Thrust >= 500 || checkY == true && obj.Thrust >= 500 || checkZ == true && obj.Thrust >= 500)
		{
			obj.isDead = true;
		}

		obj.hitbox = AABB(Vector3(obj.Pos.x - 100, obj.Pos.y - 25, obj.Pos.z - 60), Vector3(obj.Pos.x + 100, obj.Pos.y + 80, obj.Pos.z + 60));
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
		obj.Pitch -= dt * 50.f;
		rotation.SetToRotation(-(dt * 50.f), obj.Right.x, obj.Right.y, obj.Right.z);
		obj.View = rotation * obj.View;
		obj.Up = rotation * obj.Up;
		obj.Right = rotation * obj.Right;
	}

	if (Application::IsKeyPressed('S'))
	{
		Mtx44 rotation;
		obj.Pitch += dt * 50.f;
		rotation.SetToRotation(dt * 50.f, obj.Right.x, obj.Right.y, obj.Right.z);
		obj.View = rotation * obj.View;
		obj.Up = rotation * obj.Up;
		obj.Right = rotation * obj.Right;
	}

	if (Application::IsKeyPressed('A'))
	{
		Mtx44 rotation;
		obj.Yaw += dt * 50.f;
		rotation.SetToRotation(dt * 50.f, 0, 1, 0);
		obj.View = rotation * obj.View;
		obj.Up = rotation * obj.Up;
		obj.Right = rotation * obj.Right;
	}

	if (Application::IsKeyPressed('D'))
	{
		Mtx44 rotation;
		obj.Yaw -= dt * 50.f;
		rotation.SetToRotation(-(dt * 50.f), 0, 1, 0);
		obj.View = rotation * obj.View;
		obj.Up = rotation * obj.Up;
		obj.Right = rotation * obj.Right;
	}

	if (Application::IsKeyPressed('E') && obj.Thrust < 50.f && obj.delay <= 0)
	{
		bool checkY = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (obj.hitbox.AABBtoAABB(hitbox[i], Vector3(0, -2, 0)) && checkY == false)
			{
				checkY = true;
			}
		}
		if (checkY == true)
		{
			obj.Pitch = 0;
			obj.Thrust = 0;
			obj.View.y = 0;
			camera.position = obj.Pos - Vector3(0, 10, -30);
			camera.target = camera.position + Vector3(obj.View.x, 0, obj.View.z);
			camera.view = obj.View;
			camera.right = obj.Right;
			camera.up = camera.right.Cross(camera.view);
			obj.interaction = AABB(Vector3(obj.Pos.x - 20, obj.Pos.y - 30, obj.Pos.z - 20), Vector3(obj.Pos.x + 20, obj.Pos.y, obj.Pos.z + 20));
			obj.board = false;
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
		camera.target.y += camera.view.y * dt * camera.cameraSpeed;
		camera.position.y += camera.view.y * dt * camera.cameraSpeed;
		camera.target.x += camera.view.x * dt * camera.cameraSpeed;
		camera.position.x += camera.view.x * dt * camera.cameraSpeed;
		camera.target.z += camera.view.z * dt * camera.cameraSpeed;
		camera.position.z += camera.view.z * dt * camera.cameraSpeed;
	}

	// Walking backward and checking with hitboxes
	if (Application::IsKeyPressed('S'))
	{
		camera.target.x -= camera.view.x * dt * camera.cameraSpeed;
		camera.position.x -= camera.view.x * dt * camera.cameraSpeed;
		camera.target.y -= camera.view.y * dt * camera.cameraSpeed;
		camera.position.y -= camera.view.y * dt * camera.cameraSpeed;
		camera.target.z -= camera.view.z * dt * camera.cameraSpeed;
		camera.position.z -= camera.view.z * dt * camera.cameraSpeed;
	}

	// Walking left and checking with hitboxes
	if (Application::IsKeyPressed('A'))
	{
		camera.target.z -= camera.right.z * dt * camera.cameraSpeed;
		camera.position.z -= camera.right.z * dt * camera.cameraSpeed;
		camera.target.y -= camera.right.y * dt * camera.cameraSpeed;
		camera.position.y -= camera.right.y * dt * camera.cameraSpeed;
		camera.target.x -= camera.right.x * dt * camera.cameraSpeed;
		camera.position.x -= camera.right.x * dt * camera.cameraSpeed;
	}

	// Walking Right and checking with hitboxes
	if (Application::IsKeyPressed('D'))
	{
		camera.target.z += camera.right.z * dt * camera.cameraSpeed;
		camera.position.z += camera.right.z * dt * camera.cameraSpeed;
		camera.target.y += camera.right.y * dt * camera.cameraSpeed;
		camera.position.y += camera.right.y * dt * camera.cameraSpeed;
		camera.target.x += camera.right.x * dt * camera.cameraSpeed;
		camera.position.x += camera.right.x * dt * camera.cameraSpeed;
	}
}
