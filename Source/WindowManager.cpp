#include "WindowManager.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace glm;

WindowManager::WindowManager(uint32_t width, uint32_t height)
{
	GetDisplay();
	memset(&state, 0, sizeof(DisplayState));

	state.width = width;
	state.height = height;
	
	InitOpenGL();
}

WindowManager::~WindowManager()
{
}

bool WindowManager::GetDisplay()
{
	/*
	 * X11 native display initialization
	 */

	x_display = XOpenDisplay(NULL);
	if (x_display == NULL)
	{
		return false;
	}

	root = DefaultRootWindow(x_display);
	return true;
}

void WindowManager::InitOpenGL()
{

	XSetWindowAttributes swa;
	XSetWindowAttributes  xattr;
	XWMHints hints;
	XEvent xev;

#define ES_WINDOW_RGB           0

	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;

	root = DefaultRootWindow(x_display);

	swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask;
	swa.background_pixmap = None;
	swa.background_pixel = 0;
	swa.border_pixel = 0;
	swa.override_redirect = true;

	win = XCreateWindow(
		x_display,
		root,
		0,		// puts it at the top left of the screen
		0,
		state.width,	//set size  
		state.height,
		0,
		CopyFromParent,
		InputOutput,
		CopyFromParent,
		CWEventMask,
		&swa);

	if (win == 0)
	{
		exit(0);       // we need to trap this;
	}

	Atom wm_state = XInternAtom(x_display, "_NET_WM_STATE", False);
	Atom fullscreen = XInternAtom(x_display, "_NET_WM_STATE_FULLSCREEN", False);

	XChangeProperty(x_display, win, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&fullscreen, 1);

	int screen = DefaultScreen(x_display);
	// Get screen width and height
	int screenWidth = DisplayWidth(x_display, screen);
	int screenHeight = DisplayHeight(x_display, screen);

	state.height = screenHeight;
	state.width = screenWidth;

	XMoveResizeWindow(x_display, win, 0, 0, screenWidth, screenHeight);

	state.nativewindow = (EGLNativeWindowType)win;

	XSelectInput(x_display, win, KeyPressMask | KeyReleaseMask | ExposureMask | KeyPress);
	xattr.override_redirect = 1; // 1 = true
	XChangeWindowAttributes(x_display, win, CWOverrideRedirect, &xattr);

	hints.input = 1;
	hints.flags = InputHint;
	XSetWMHints(x_display, win, &hints);

	XSelectInput(x_display, win, KeyPress | KeyRelease | MotionNotify);


	char* title = (char*)"x11 Demo";
	// make the window visible on the screen
	XMapWindow(x_display, win);
	XStoreName(x_display, win, title);

	// get identifiers for the provided atom name strings
	wm_state = XInternAtom(x_display, "_NET_WM_STATE", 0);

	memset(&xev, 0, sizeof(xev));
	xev.type = ClientMessage;
	xev.xclient.window = win;
	xev.xclient.message_type = wm_state;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = 1;
	xev.xclient.data.l[1] = 0;
	XSendEvent(
		x_display,
		DefaultRootWindow(x_display),
		0,
		SubstructureNotifyMask,
		&xev);

	XFlush(x_display);
	// end of xdisplay

	// Get EGLDisplay	
	egldisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);       //eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (egldisplay == EGL_NO_DISPLAY)
	{
		exit(0);          // we need to trap this;
	}

	// Initialize EGL
	if (!eglInitialize(egldisplay, &majorVersion, &minorVersion))
	{
		EGLint err = eglGetError();    // should be getting error values that make sense now
		exit(err);            // we need to trap this;
	}

	// Get configs
	if (!eglGetConfigs(egldisplay, NULL, 0, &numConfigs))
	{
		EGLint err = eglGetError();
		exit(err);            // we need to trap this;
	}


	// Choose config
	if (!eglChooseConfig(egldisplay, attribute_list, &config, 1, &numConfigs))
	{
		EGLint err = eglGetError();
		exit(err);            // we need to trap this;
	}

	// Create a GL context
	context = eglCreateContext(egldisplay, config, NULL, GiveMeGLES3);
	// context = eglCreateContext(egldisplay, config, EGL_NO_CONTEXT, GiveMeGLES2);

	if (context == EGL_NO_CONTEXT)
	{
		EGLint err = eglGetError();
		exit(err);             // we need to trap this;
	}

	// Create a surface
	surface = eglCreateWindowSurface(egldisplay, config, state.nativewindow, NULL);     // this fails with a segmentation error?

	if (surface == EGL_NO_SURFACE)
	{
		EGLint err = eglGetError();
		exit(err);               // we need to trap this;
	}

	// Make the context current
	if (!eglMakeCurrent(egldisplay, surface, surface, context))
	{
		EGLint err = eglGetError();
		exit(err);            // we need to trap this;
	}

	state.display = egldisplay;
	state.surface = surface;
	state.context = context;

	// EGLBoolean test = eglSwapInterval(egldisplay, 01);        // 1 to lock speed to 60fps (assuming we are able to maintain it), 0 for immediate swap (may cause tearing) which will indicate actual frame rate
	// on xu4 this seems to have no effect

	// Some OpenGLES2.0 states that we might need
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(1);
	glDepthRangef(0.0f, 1.0f);
	glClearDepthf(1.0f);

	int t; // a dum,ing variable to extra some useful data

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &t);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &t);

	glViewport(0, 0, state.width, state.height);
	glClearColor(0.2f, 0.2f, 0.2f, 1);

	glCullFace(GL_BACK);
}
