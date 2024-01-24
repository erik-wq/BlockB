#pragma once
#include <LinearMath/btIDebugDraw.h>

#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"

#if defined(__arm__) || defined(__aarch64__)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#else
#include "glad/glad.h"
#endif

#include <iostream>
#include <vector>


class Shader;

struct DebugLine
{
    btVector3 from;
    btVector3 to;
};

class DebugDrawer : public btIDebugDraw {
public:
    DebugDrawer(Shader* debugShader);
    virtual ~DebugDrawer();

    void RenderDebug(glm::mat4 MVP);

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

    void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar, int, const btVector3& color) override {
        drawLine(pointOnB, pointOnB + (normalOnB * 2), color);
    }

    void reportErrorWarning(const char* warningString) override {
        std::cerr << "Bullet Warning: " << warningString << std::endl;
    }

    void draw3dText(const btVector3&, const char*) override {
        
    }

    void setDebugMode(int debugMode) override {
        debugMode_ = debugMode;
    }

    int getDebugMode() const override {
        return debugMode_;
    }

private:
    int debugMode_{ btIDebugDraw::DBG_DrawWireframe };
    
    Shader* shader;

    std::vector<DebugLine> lines;

    GLuint VAO;
    GLuint VBO;

    btVector3 debugColor;
};