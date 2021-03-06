#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "Scene.h"

class Application
{
	
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	
	static bool quit;
	static int mouseScrollingUp; //0 - No Scroll, 1 - Scroll Up, 2 - Scroll Down

	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool IsKeyReleased(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static bool MouseScrollDirection();
	static void GetCursorPos(double *xpos, double *ypos);
	static int GetWindowWidth();
	static int GetWindowHeight();



private:
	Application();
	~Application();

	//Declare a window object
	StopWatch m_timer;

};

#endif