#include <vector>
#include <queue>
#include "Vector3.h"
#include "MyMath.h"


using std::vector;
using std::queue;


class pathFinding{
public:
	pathFinding();
	pathFinding(Vector3 location);
	virtual ~pathFinding();


	void pathFinding::pathRoute(queue<Vector3>& wayPoints, float dt);
	float distanceBetween2points(Vector3 Point1, Vector3 Point2);


private:


	Vector3 initialLocation;


};

