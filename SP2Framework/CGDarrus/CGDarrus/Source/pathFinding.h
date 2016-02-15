#include <vector>
#include <queue>
#include "Vector3.h"
#include "MyMath.h"
#include "Application.h"


using std::vector;
using std::queue;


class pathFinding{
public:
	pathFinding();
	virtual ~pathFinding();



private:

	Vector3 pathRoute(Vector3 initialLocation, queue<Vector3>& wayPoints, float dt);
};

