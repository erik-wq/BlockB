#pragma once
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <pthread.h>
#include <linux/input.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <queue>
#include "EventManager.h"

#define Pressed 1
#define Released 0

struct MouseData
{
	int deltax;
	int deltay;
	int posx;
	int posy;
};

class Input
{
public:
	Input();
	~Input();

	MouseData GetMouseData();
	
	bool Init(Display* display, Window* window);

	static void* ProcessInputThread(void* arg);

	void CheckMousePos(int x, int y);

	void UpdateKey(int state, char key);

	bool GetKey(char key) { return keys[key]; }

	void UpdateButton(int state, char button);

	bool GetButton(char key) { return buttons[key - 1]; }
	
	pthread_t threadKeyboard;
	 
	Display* dis;

	Window* root;

	int width;
	int height;
private:
	Window* win;

	char keys[255];
	Cursor cursor;
	Pixmap pixmap;

	int mouseX = 0;
	int mouseY = 0;
	int mouseDeltaX = 0;
	int mouseDeltaY = 0;

	char buttons[3];
};
