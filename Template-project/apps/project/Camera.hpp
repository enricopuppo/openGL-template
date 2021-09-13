#ifndef CAMERA_H
#define CAMERA_H

#include "Player.hpp"
#include "Quaternion.hpp"
#include "ArcBall.hpp"
#include "./glm/glm.hpp"
#include <GLFW/glfw3.h>

class Camera
{
private:
    glm::vec3 position;
    float pitch;
    float yaw;
    float roll;
    float angleAround;
    float distance;
    ArcBall arcball;
    GLFWwindow *window;
    int screenWidth;
    int screenHeight;
    Player *player;
    bool firstPersonView;
    glm::mat4 viewMatrix;
    float *scrollDelta;
    double *pointerPositionX;
    double *pointerPositionY;
    float *pointerDeltaX;
    float *pointerDeltaY;
    bool *isSpacePressed;
    bool *clicked;

    void calculateZoom();
    void calculatePitch();
    void calculateAngleAround();
    void calculateCameraPos();
    glm::vec2 screenToNorm(float posX, float posY);

public:
    Camera(Player *player, DisplayManager &dm, float *scrollDelta, double *pointerPositionX, double *pointerPositionY,
           float *pointerDeltaX, float *pointerDeltaY, bool *isSpacePressed, bool *clicked);
    void move();
    bool isFirstPersonViewEnabled();
    void toggleFirstPersonView();
    void invertPitch();
    void resetPitch();
    float getDistance();
    glm::mat4 getViewMatrix();
    glm::vec3 getPosition();
    float getPitch();
    float getYaw();
    float getRoll();
};

#endif