#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "WindowsInput.h"
#include "WindowsWindow.h"

#include "Application/GameApp.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include <iostream>

typedef unsigned int uint;

// settings
const uint SCR_WIDTH = 1280;
const uint SCR_HEIGHT = 1024;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

const uint sizePixels = 24;
int main()
{
    WindowsWindow* window = new WindowsWindow(SCR_WIDTH, SCR_HEIGHT, "Pitfall3D");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    WindowsInput* input = new WindowsInput(window->GetWindow());

    ScreenSize state = window->GetScreenSize();

    GameApp* game = new GameApp(input, window);

    // Create ImGui context
    ImGui::CreateContext();

    // Set ImGui context as current
    ImGui::SetCurrentContext(ImGui::GetCurrentContext());
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsClassic();

    ImGuiIO& io = ImGui::GetIO();

    // ImFont* font1 = io.Fonts->AddFontFromFileTTF("../Game/Resources/ImGui/PixelifySans-Regular.ttf", sizePixels);

    io.DisplaySize.x = static_cast<float>(state.widht); // Set to your actual width
    io.DisplaySize.y = static_cast<float>(state.height); // Set to your actual height

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);

    float fps = 0;
    float deltaTime = 1;
    float lastFrame = 1;
    while (!game->GetEndApplication())
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        fps = 1.0f / deltaTime;

        // std::cout << fps << std::endl;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        io.DeltaTime = deltaTime;

        // game Tick/Update
        game->Tick(deltaTime);

        // render objects
        game->Render();
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    delete game;
    delete input;
    delete window;

    return 0;
}

