#include "DebugDrawer.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include "Shader.h"
#include <string>

DebugDrawer::DebugDrawer() : btIDebugDraw()
{
    shader = new Shader("../../../Resources/Shaders/debug.vert", "../../../Resources/Shaders/debug.frag");
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

DebugDrawer::~DebugDrawer()
{
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete shader;
}

void DebugDrawer::RenderDebug(glm::mat4 MVP)
{
    if (lines.size() == 0) return;

    glUseProgram(shader->ID);

    shader->setMat4("projectionMatrix", MVP);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(DebugLine),
        &lines[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
        (void*)0);

    glDrawArrays(GL_LINES, 0, lines.size() * 2);

    glBindVertexArray(0);

    lines.clear();
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    // draws a simple line of pixels between points but stores them for later draw
    lines.push_back(DebugLine{from, to});
}