#pragma once

// Graphics class primary responsibility is to set up OpenGLES for render and to hold info on X11 window
#include <stdio.h>
#include <assert.h>
#include <string>
#include <math.h>
#include <sys/time.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <vector>
// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// these are more focused on X11 for display so we don't need to include them on an older Raspbery123, the Xlib and XrandR gives us all the Raspberry needs
#include  <X11/Xatom.h>
#include  <X11/Xutil.h>

#include "App/WindowSystem.h"

struct DisplayState
{
	uint32_t width;
	uint32_t height;

	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	// slight difference in the kind of window needed for X11 and Rpi	R
	EGLNativeWindowType  nativewindow;
};

// define what kind of EGL config we want, we can add more options but the more we add the less likely it might be available across systems
static const EGLint attribute_list[] =
{
	EGL_RED_SIZE, 8,
	EGL_GREEN_SIZE, 8,
	EGL_BLUE_SIZE, 8,
	EGL_ALPHA_SIZE, 8,
	EGL_DEPTH_SIZE,	8, // << this is a useful one to reseach
	EGL_SURFACE_TYPE,
	EGL_WINDOW_BIT,
	EGL_CONFORMANT,
	EGL_OPENGL_ES3_BIT_KHR,
	//be aware, some emulated Mesa OpenGLES2.0 drivers, can choke on these two lines
	// and on Pi4 ES3.x and X11 it really bites on the FPS. How many will notice this and report the difference?
	EGL_SAMPLE_BUFFERS, 1,		// if you want anti alias at variable fps cost
	EGL_SAMPLES, 4,				//keep these 2lines, especially useful for lower resolution

	EGL_NONE
};

static const EGLint GiveMeGLES3[] = {
	EGL_CONTEXT_MAJOR_VERSION_KHR,
	3,
	EGL_CONTEXT_MINOR_VERSION_KHR,
	1,
	EGL_NONE,
	EGL_NONE
};

static const EGLint GiveMeGLES2[] =
{
	EGL_CONTEXT_CLIENT_VERSION, 2,
	EGL_NONE
};

class WindowManager : public WindowSystem
{
public:
	WindowManager(uint32_t width, uint32_t height);
	virtual ~WindowManager();
	DisplayState* GetDisplayState() { return &state; }
	Display* GetCurrentDisplay() { return x_display; }
	Window* GetRootWindow() { return &root; }

	virtual void SwapBuffers() override;

	void ImGuiImplX11();
private:
	bool GetDisplay();
	void InitOpenGL();
protected:
	// we can store all the possible resolutions here		
	short   possible_frequencies[64][64]; // a big list of details
	int     num_rates; // how many possible rates
	int		num_sizes; // how many possible sizes
	int		FPS = 0;// we might display it or need it
	Display* x_display;
	Window win;
	Window root;

	uint32_t ResolutionX = 1024;
	uint32_t ResolutionY = 768;
	EGLDisplay egldisplay;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	EGLint num_config;
	DisplayState state;
};

