#pragma once
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <pthread.h>
#include <linux/input.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include "App/Event.h"
#include "App/InputSystem.h"

class Input : public InputSystem
{
public:
	Input();
	virtual ~Input();
	
	bool Init(Display* display, Window* window);

	static void* ProcessInputThread(void* arg);

	void CheckMousePos(int x, int y);

	void UpdateKey(int state, char key);

	void UpdateButton(int state, char button);
	
	virtual bool GetKey(char key) override;
	void BackspacePressed(bool);

	bool GetButton(char key) override;

	pthread_t threadKeyboard;
	 
	Display* dis;

	Window* root;

	int width;
	int height;
private:
	int buttons[3] = { 0 };

	int keys[256] = { 0 };

	Window* win;

	Cursor cursor;
	Pixmap pixmap;
};
