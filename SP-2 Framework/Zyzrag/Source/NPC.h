#include "Object.h"
#include "MyMath.h"

using std::string;

class NPC : public Object
{
public:
	NPC();
	NPC(Vector3 pos, Vector3 view, float NPCsize, float hitboxSize, float maxDistance, int mesh);
	~NPC();

	void update(double dt);

	string questCompleteDialogue, questDialogue, questName, questStatus;

	float curDistance, NPCsize, yaw, rotateHand, rotateLeg, count;
	int maxDistance, rotateDir, mesh;
	bool reward, complete;
};