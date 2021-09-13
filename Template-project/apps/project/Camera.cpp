#include "Camera.hpp"
#include "Player.hpp"
#include "Quaternion.hpp"
#include "ArcBall.hpp"
#include "./glm/glm.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include "./glm/gtx/string_cast.hpp"

Camera::Camera(Player* player, DisplayManager &dm, float* scrollDelta, double* pointerPositionX, double* pointerPositionY, float* pointerDeltaX, float* pointerDeltaY, bool* isSpacePressed, bool* clicked){
    this->position = glm::vec3(0.0, 5.0, 10.0);
    this->pitch = 25.0;
    this->yaw = 0.0;
    this->roll = 0.0;
    this->angleAround = 0.0;
    this->distance = 1.0;

    this->arcball = ArcBall(position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    this->window = dm.getWindow();
    this->screenWidth = dm.getScreenWidth();
    this->screenHeight = dm.getScreenHeight();
    this->player = player;
    this->firstPersonView = false;

    this->scrollDelta = scrollDelta;
    this->pointerPositionX = pointerPositionX;
    this->pointerPositionY = pointerPositionY;
    this->pointerDeltaX = pointerDeltaX;
    this->pointerDeltaY = pointerDeltaY;
    
    this->isSpacePressed = isSpacePressed;
    this->clicked = clicked;
}

// according to the active view updates the camera position
void Camera::move(){
    if(*isSpacePressed){
        toggleFirstPersonView();
        *isSpacePressed = false;
    }
    if(isFirstPersonViewEnabled()){     // no zoom, euler angles
        calculatePitch();
        calculateAngleAround();
        calculateCameraPos();
        this->yaw = 180.0 - ((*player).getRotation()[1] + angleAround);
    }
    else{       // zoom, quaternions
        calculateZoom();        
        if(*clicked){
            arcball.setMouseStart(screenToNorm(*pointerPositionX, *pointerPositionY));
            *clicked = false;
        }
        if(*pointerDeltaX != 0.0 || *pointerDeltaY != 0.0){
            arcball.rotate(glm::vec2(screenToNorm(*pointerPositionX, *pointerPositionY)));
            *pointerDeltaX = *pointerDeltaY = 0.0;
        }
        this->viewMatrix = arcball.getViewMatrix();
    }
}

// using the variable updated by the mouse scroll callback modifies the zoom level
// of the third person view. It is capped to avoid going inside the submarine and/or
// going too far
void Camera::calculateZoom(){
    float updatedDistance = distance - *scrollDelta;
    if(updatedDistance < -9.0)
        distance = -9.0;
    else if(updatedDistance >= 20.0)
        distance = 20.0;
    else{
        distance = updatedDistance;
        arcball.zoom(*scrollDelta);
    }
    *scrollDelta = 0.0;
}

// using the variable updated by the mouse move callback modifies the pitch angle
// of the camera (vertical). It is capped to simulate a submarine view
void Camera::calculatePitch(){
    float updatedPitch = pitch - *pointerDeltaY * 0.2;
    if(updatedPitch < 0.35)
        pitch = 0.35;
    else if(updatedPitch > 90.0)
        pitch = 90.0;
    else
        pitch = updatedPitch;
    *pointerDeltaY = 0.0;
}

// using the variable updated by the mouse move callback modifies the around angle.
// It is capped to simulate a submarine view
void Camera::calculateAngleAround(){
    angleAround += *pointerDeltaX * 0.15;
    float updatedAngleAround = angleAround + *pointerDeltaX * 0.15;
    if(updatedAngleAround < -45.0)
        angleAround = -45.0;
    else if(updatedAngleAround > 45.0)
        angleAround = 45.0;
    else
        angleAround = updatedAngleAround;
    *pointerDeltaX = 0.0;
}

// places the camera exactly in the same place as the submarine
void Camera::calculateCameraPos(){
    glm::vec3 playerPos = (*player).getPosition();
    this->position[0] = playerPos[0];
    this->position[1] = playerPos[1];
    this->position[2] = playerPos[2];
}

bool Camera::isFirstPersonViewEnabled(){
    return firstPersonView;
}

// resets the initial values for both views
void Camera::toggleFirstPersonView(){
    if(firstPersonView){
        this->position = glm::vec3(0.0, 5.0, 10.0);
        this->arcball = ArcBall(position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        this->viewMatrix = arcball.getViewMatrix();
        // this->pitch = 25.0;
        this->distance = 1.0;
    }
    else{
        this->angleAround = 0.0;
        this->pitch = 5.0;
    }
    firstPersonView = !firstPersonView;
}

// converts screen coordinates (pixel) into normalized display coords [-1,1]
glm::vec2 Camera::screenToNorm(float posX, float posY){
    return glm::vec2(
        posX * 2.0 / screenWidth - 1.0,
        1.0 - posY * 2.0 / screenHeight
    );
}

// places the camera in the same position, but below the ground level
void Camera::invertPitch(){
    arcball.invertPitch();
    viewMatrix = arcball.getViewMatrix();
}

// resets the camera above ground level
void Camera::resetPitch(){
    arcball.resetPitch();
    viewMatrix = arcball.getViewMatrix();
}

glm::mat4 Camera::getViewMatrix(){
    return viewMatrix;
}

glm::vec3 Camera::getPosition(){
    return position;
}

float Camera::getPitch(){
    return pitch;
}

float Camera::getYaw(){
    return yaw;
}

float Camera::getRoll(){
    return roll;
}

float Camera::getDistance(){
    return distance;
}