#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "stb_image.h"

class DisplayManager
{
private:
    int screenWidth;
    int screenHeight;
    GLFWwindow *window;
    long int lastFrameTime;
    float deltaTime;
    float scrollDeltaX = 0.0;
    float scrollDeltaY = 0.0;

    void initGLFW();
    void initGLAD();
    GLFWwindow *createWindow();
    void initImGui(GLFWwindow *window);
    long int getCurrentTime();

public:
    DisplayManager();
    DisplayManager(int width, int height);
    void setupGlfwCallbacks();
    void updateTime();
    void init();
    void clean();

    GLFWwindow *getWindow();
    float getAspectRatio();
    float getDeltaTime();
    int getScreenWidth();
    int getScreenHeight();
};

#endif