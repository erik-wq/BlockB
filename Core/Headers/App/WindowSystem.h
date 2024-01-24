#pragma once

struct ScreenSize
{
	int widht;
	int height;
};

class WindowSystem
{
public:
	virtual void SwapBuffers() = 0;
	ScreenSize GetScreenSize() { return screenSize; }
	virtual void Clear() {};
protected:
	ScreenSize screenSize = {0,0};
};