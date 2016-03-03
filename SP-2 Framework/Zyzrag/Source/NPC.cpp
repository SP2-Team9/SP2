#include "NPC.h"


NPC::NPC()
{

}

NPC::NPC(Vector3 pos, Vector3 view, float NPCsize, float hitboxSize, float maxDistance, int mesh)
{
	this->mesh = mesh;
	SetPos(pos);
	SetHitboxSize(hitboxSize);
	SetView(view);
	this->NPCsize = NPCsize;
	complete = false;
	reward = false;
	curDistance = 0;
	this->maxDistance = maxDistance;
	questStatus = "";
	rotateHand = 0;
	rotateLeg = 0;
	rotateDir = 1;

	yaw = Math::RadianToDegree(acos(Vector3(0,0,1).Dot(view)));
}

NPC::~NPC()
{

}

void NPC::update(double dt)
{	
	if (complete == false)
		questStatus = "In Progress";
	else
	{
		questStatus = "Complete";
	}
		

	
	
	if (rotateHand * rotateDir > 15)
		rotateDir = -rotateDir;
	if (rotateLeg * rotateDir > 15)
		rotateDir = -rotateDir;

	rotateHand += rotateDir * dt * 100;
	rotateLeg += rotateDir * dt * 100;

	if (curDistance > maxDistance)
	{
		curDistance = 0;
		View *= -1;
	}
	curDistance += dt * View.getMagnitude();
	Pos += View * dt;
	updateHitbox();
}