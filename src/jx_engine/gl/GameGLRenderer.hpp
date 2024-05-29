#ifndef GAMERENDERER_hpp
#define GAMERENDERER_hpp

#include <memory>
#include <glad/glad.h>

#include "core/gl/Camera.h"
#include "core/gl/shaderClass.h"
#include "core/gl/BulletDebugDrawer.cpp"

class GameGLRenderer {

    public:
        GameGLRenderer(int winWidth, int winHeight, Camera* cam);

        BulletDebugDrawer* debugDrawer;

        GLint colorUniformLocation; //Debug purposes, might move
        GLint modelMatrixLOC;
        GLint useTextureLOC;

        void RenderPrep();
        std::shared_ptr<Shader> mainShader;

    private:
        Camera* camera;


        float lightCtr = 0.0f;
};

#endif