#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.hpp"
#include "TexturedModel.hpp"
#include "DisplayManager.hpp"
#include "./glm/glm.hpp"
#include <GLFW/glfw3.h>
#include "Light.hpp"

class Player : public Entity
{
private:
    float movementSpeed;
    float turnSpeed;
    float upSpeed;
    float currentUpSpeed;
    float currentMovementSpeed;
    float currentTurnSpeed;
    float gravity;
    float floorHeight;
    float waterHeight;
    float xLimit;
    float zLimit;

    void checkInputs(GLFWwindow *window);

public:
    Player() = default;
    Player(TexturedModel &model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 poolScale);
    glm::vec3 move(DisplayManager &dm);
};

#endif