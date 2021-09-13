#include "DisplayManager.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "stb_image.h"
#include <sys/time.h>
DisplayManager::DisplayManager()
{
    // default constructor that assigns fixed a screen size to the fields
    screenWidth = 1280;
    screenHeight = 720;
}

DisplayManager::DisplayManager(int width, int height)
{
    // second constructor that assigns to the filelds the specified screen measures
    screenWidth = width;
    screenHeight = height;
}

void DisplayManager::initGLFW()
{
    // initializes GLFW
    if (!glfwInit())
        printf("ERROR during GLFW initialization\n");
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void DisplayManager::initGLAD()
{
    // initializes GLAD
    if (!gladLoadGL())
        printf("ERROR during GLAD initialization\n");
}

GLFWwindow *DisplayManager::createWindow()
{
    // creates a viewport with the specified screen size and a new window in it
    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "CGAR Project", NULL, NULL);
    lastFrameTime = getCurrentTime();
    if (window)
        glfwMakeContextCurrent(window);
    return window;
}

void DisplayManager::initImGui(GLFWwindow *window)
{
    // initializes ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifndef __APPLE__
    ImGui_ImplOpenGL3_Init();
#else
    ImGui_ImplOpenGL3_Init("#version 330");
#endif
}

void DisplayManager::init()
{
    // initializes all the components
    initGLFW();
    window = createWindow();
    initGLAD();
    glViewport(0, 0, screenWidth, screenHeight);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    initImGui(window);
}

void DisplayManager::clean()
{
    // destroys the window and closes GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow *DisplayManager::getWindow()
{
    return window;
}

float DisplayManager::getAspectRatio()
{
    return float(screenWidth) / float(screenHeight);
}

long int DisplayManager::getCurrentTime()
{
    // returns the actual time in milliseconds
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000; // in msec
}

void DisplayManager::updateTime()
{
    // updates the current time variable and the delta one
    // respect to the previous frame rendered
    long int currentTime = getCurrentTime(); // sec
    deltaTime = (currentTime - lastFrameTime) / 1000.0;
    lastFrameTime = currentTime;
}

float DisplayManager::getDeltaTime()
{
    return deltaTime;
}

int DisplayManager::getScreenWidth()
{
    return screenWidth;
}

int DisplayManager::getScreenHeight()
{
    return screenHeight;
}