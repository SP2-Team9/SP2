
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Vector3.h"
#include "timer.h"

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

private:

	//Declare a window object
	StopWatch m_timer;
	//const GLFWvidmode * mode;
};

#endif