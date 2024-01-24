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

#include <time.h>

#include <vector>
// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "WindowManager.h"
#include "Application/GameApp.h"
#include "Input.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"

const uint sizePixels = 24;

const uint SCR_WIDTH = 1280;
const uint SCR_HEIGHT = 1024;

int main(int argc, char *argv[])
{
	WindowManager* window = new WindowManager(SCR_WIDTH, SCR_HEIGHT);
	Input* input = new Input();

	DisplayState* state = window->GetDisplayState();
	input->root = window->GetRootWindow();
	input->Init(window->GetCurrentDisplay(), window->GetRootWindow());

	GameApp* game = new GameApp(input, window);

	// Create ImGui context
	ImGui::CreateContext();

	// Set ImGui context as current
	ImGui::SetCurrentContext(ImGui::GetCurrentContext());
	ImGui_ImplOpenGL3_Init("#version 300 es");
	ImGui::StyleColorsClassic();

	ImGuiIO& io = ImGui::GetIO();

	// ImFont* font1 = io.Fonts->AddFontFromFileTTF("../../../Game/Resources/ImGui/PixelifySans-Regular.ttf", sizePixels);

	io.DisplaySize.x = static_cast<float>(state->width); // Set to your actual width
	io.DisplaySize.y = static_cast<float>(state->height); // Set to your actual height
	
	struct timespec currentTime;
	clock_gettime(CLOCK_MONOTONIC, &currentTime);
	float startTime = (float)currentTime.tv_sec + 1.0e-9f * currentTime.tv_nsec - 0.001f;

	float deltaTime = 0.0f;

	while (!game->GetEndApplication())
	{
		clock_gettime(CLOCK_MONOTONIC, &currentTime);
		float frameStartTime = (float)currentTime.tv_sec + 1.0e-9f * currentTime.tv_nsec;
		deltaTime = frameStartTime - startTime;
		startTime = frameStartTime;

		std::cout << "FPS: " << 1.0f / deltaTime << "deltaSeconds " << deltaTime << std::endl;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		window->ImGuiImplX11();
		ImGui::NewFrame();

		io.DeltaTime = deltaTime;

		// game Tick/Update
		game->Tick(deltaTime);

		// render objects
		game->Render();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	delete game;
	delete input;
	delete window;

	return 0;
}