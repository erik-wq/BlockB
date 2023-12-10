
#include "Input.h"
#include <dirent.h>
#include <sstream>
#include <string.h>

/*
* space = 57
* w = 17
* a = 30
* s = 31 
* d = 32
*/

Input::Input()
{
}

Input::~Input()
{
    XUngrabPointer(dis, CurrentTime);
    XFreePixmap(dis, pixmap);
    XFreeCursor(dis, cursor);
}

MouseData Input::GetMouseData()
{
    MouseData data = MouseData{ mouseDeltaX, mouseDeltaY, mouseX, mouseY };
    mouseDeltaX = 0;
    mouseDeltaY = 0;
    return data;
}

void* Input::ProcessInputThread(void* arg)
{
    XEvent event;
    KeySym key;
    char text[255];
    while (1)
    {
        XNextEvent(((Input*)arg)->dis, &event);
        if (event.type == KeyPress && XLookupString(&event.xkey, text, 255, &key, 0) == 1)
        {
            ((Input*)arg)->UpdateKey(Pressed, text[0]);
        }
        else if (event.type == KeyRelease && XLookupString(&event.xkey, text, 255, &key, 0) == 1)
        {
            ((Input*)arg)->UpdateKey(Released, text[0]);
        }
        else if (event.type == ButtonRelease)
        {
            ((Input*)arg)->UpdateButton(Pressed, event.xbutton.button);
        }
        else if (event.type == ButtonPress)
        {
            ((Input*)arg)->UpdateButton(Released, event.xbutton.button);
        }
        if (event.type == MotionNotify)
        {
            ((Input*)arg)->CheckMousePos(event.xmotion.x, event.xmotion.y);
        }
    }
    pthread_exit(NULL);
}

void Input::CheckMousePos(int x, int y)
{
    // accumulate total delta over time
    mouseDeltaX += x - mouseX;
    mouseDeltaY += y - mouseY;
    mouseX = x;
    mouseY = y;
    // XWarpPointer(dis, *win, *win, 0, 0, width, height, width / 2, height / 2);
}

void Input::UpdateKey(int state, char key)
{
    keys[key] = state;
}

void Input::UpdateButton(int state, char button)
{
    if (button > 3)
    {
        printf("outside of bounds %d \n", button);
        return;
    }
    buttons[button - 1] = state;
}

// maybe change window to reference
bool Input::Init(Display* display, Window* window)
{
    dis = display;
    win = window;
	int result;

    XEvent ev;
    XQueryPointer(display, *window, &ev.xbutton.root, &ev.xbutton.window,
        &ev.xbutton.x_root, &ev.xbutton.y_root,
        &ev.xbutton.x, &ev.xbutton.y,
        &ev.xbutton.state);

    mouseX = ev.xbutton.x_root;
    mouseY = ev.xbutton.y_root;

    pixmap = XCreatePixmap(display, *window, 20, 20, 1);
    GC gc = XCreateGC(display, pixmap, 0, NULL);

    XSetForeground(display, gc, 1); // 1 is white, representing the red pixel
    XFillRectangle(display, pixmap, gc, 0, 0, 20, 20);

    XFreeGC(display, gc);

    XColor redColor;
    redColor.red = 65535;    // Full intensity for red
    redColor.green = 0;      // No intensity for green
    redColor.blue = 0;       // No intensity for blue
    redColor.flags = DoRed | DoGreen | DoBlue;

    // Create a cursor from the pixmap
    Cursor redCursor = XCreatePixmapCursor(display, pixmap, pixmap, &redColor, &redColor, 10, 10);

    // Set the window cursor to the custom red cursor
    XDefineCursor(display, *window, redCursor);


    XGrabPointer(display, *win, True, ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
        GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
    
    // XWarpPointer(dis, *win, *win, 0, 0, width, height, width / 2, height / 2);

	result = pthread_create(&threadKeyboard, NULL, &ProcessInputThread, this);
    if (result != 0)	return false;

	return true;
}
