#ifndef APPLICATION_H
#define APPLICATION_H

#include "Vector3.h"
#include "timer.h"
#include "sharedData.h"

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	
	static bool IsKeyPressed(unsigned short key);
	static void getMouse(double & x, double & y);
	static void centerMouse();
	static void hideMouse();
	static void showMouse();

	static int screenWidth;
	static int screenHeight;

	static double scrollX;
	static double scrollY;

private:

	//Declare a window object
	StopWatch m_timer;
};

#endif