#include "WindowManager.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "imgui.h"
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

void WindowManager::SwapBuffers()
{
	glFlush();
	eglSwapBuffers(state.display, state.surface);
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

	screenSize.widht = state.width;
	screenSize.height = state.height;

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

	// Get EGLDisplay	
	egldisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (egldisplay == EGL_NO_DISPLAY)
	{
		exit(0);
	}

	// Initialize EGL
	if (!eglInitialize(egldisplay, &majorVersion, &minorVersion))
	{
		EGLint err = eglGetError();
		exit(err);
	}

	// Get configs
	if (!eglGetConfigs(egldisplay, NULL, 0, &numConfigs))
	{
		EGLint err = eglGetError();
		exit(err);
	}


	// Choose config
	if (!eglChooseConfig(egldisplay, attribute_list, &config, 1, &numConfigs))
	{
		EGLint err = eglGetError();
		exit(err);
	}

	// Create a GL context
	context = eglCreateContext(egldisplay, config, NULL, GiveMeGLES3);

	if (context == EGL_NO_CONTEXT)
	{
		EGLint err = eglGetError();
		exit(err);
	}

	// Create a surface
	surface = eglCreateWindowSurface(egldisplay, config, state.nativewindow, NULL);

	if (surface == EGL_NO_SURFACE)
	{
		EGLint err = eglGetError();
		exit(err);
	}

	if (!eglMakeCurrent(egldisplay, surface, surface, context))
	{
		EGLint err = eglGetError();
		exit(err);
	}

	state.display = egldisplay;
	state.surface = surface;
	state.context = context;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(1);
	glDepthRangef(0.0f, 1.0f);
	glClearDepthf(1.0f);

	glViewport(0, 0, state.width, state.height);
	glClearColor(0.2f, 0.2f, 0.2f, 1);

	glCullFace(GL_BACK);
}

void WindowManager::ImGuiImplX11()
{
	int root_x;
	int root_y;
	int win_x;
	int win_y;
	uint mask_return;
	bool result = XQueryPointer(
		x_display,
		win,
		&root,
		&root,

		&root_x,
		&root_y,
		&win_x,
		&win_y,
		&mask_return);

	if (!result)
	{
		return;
	}

	ImGuiIO& io = ImGui::GetIO();
	// we should also pass on mouse and key info into the IO here

	io.MousePos = ImVec2(root_x, root_y);
	io.MouseDown[0] = mask_return & (1 << 8); // left
	io.MouseDown[1] = mask_return & (1 << 10); // right
}
