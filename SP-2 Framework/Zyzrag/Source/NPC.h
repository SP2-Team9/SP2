#ifndef NPC_H
#define NPC_H

#include "Object.h"
#include <string>
#include "Vector3.h"

using std::string;

class NPC
{
public:
	NPC();
	NPC(Vector3 pos);
	~NPC();

	string tgaLocation;
	Vector3 pos;

	void NPCinit();
	void Render();
	void Update(double dt);


	
	
};

#endif 