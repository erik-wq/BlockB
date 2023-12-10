/* Hello Cubes
code adapted from from OpenGL® ES 2.0 Programming Guide

Presented here for Block B as a basic Framework, Rendering method is sub optimal, please take note of that, You should impliment
your own render systems and shader managment systems when you understand the methods available, workshops will be run to discuss this

Initialises a Raspberry format EGL window for rendering and provides an example a simple shader system

Provides a skeletal framework for a processing loop 
Provides input systems for mouse and keyboard (requires init to work)
(note...Linux key reads are unpredictable, if you find keys are not working use event0 or event 1 or event2.. etc
This is a known issue, we could fix with a listener on all events but other methods are being investigated)

Provides a simple Gui demonstrated on start up which allows resolution changes and info display
Provides Graphic file loading capability via MyFiles.h a wrapper class for stb_image
GLM is used as a standard maths library, you may use your own if you wish but why???

Recommended course..
Review ObjectModel base class, provide the functions stated
Review CubeModel as a standard object type. 
Provide init update and draw routines

Review the Draw function here in HelloCubes which is the main processing loop...make it work

Project builds for Arm64 or Arm32
Rpi4 is considered an X11 system, see the info on CMGT regarding setup of systems, ensure you use X11 for render.. 

*/

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <vector>

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>
#ifdef RASPBERRY
#include "bcm_host.h"
#endif

#include <vector>
// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GameApp.h>
#include "Renderer.h"

int main(int argc, char *argv[])
{
	GameApp* game = GameApp::GetInstance();
	Renderer* renderer = new Renderer();
	struct timeval t1, t2;
	struct timezone tz;

	float deltatime = 0;
	float totaltime = 0.0f;
	unsigned int frames = 0;

	gettimeofday(&t1, &tz);

	while (1)
	{
		gettimeofday(&t2, &tz);
		deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 0.0000001f);
		t1 = t2;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// std::cout << "fps "<< 1 / deltatime  << " delta time" << deltatime << '\n';

		// game Tick/Update
		game->Tick(deltatime);

		// render objects
		renderer->Render();
	}

	delete renderer;
	delete game;
	return 0;
}
